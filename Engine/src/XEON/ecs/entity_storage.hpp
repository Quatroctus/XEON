#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include <XEON/ecs/entity.h>
#include <XEON/ecs/components.hpp>

template<typename... Args>
struct is_contained;

template<typename T, typename Head, typename... Tail>
struct is_contained<T, Head, Tail...> {
	static constexpr bool value = std::is_same<T, Head>::value || is_contained<T, Tail...>::value;
};

template<typename T>
struct is_contained<T> {
	static constexpr bool value = false;
};

template<typename... Args>
struct is_unique;

template<typename Head, typename... Tail>
struct is_unique<Head, Tail...> {
	static constexpr bool value = !is_contained<Head, Tail...>::value && is_unique<Tail...>::value;
};

template<>
struct is_unique<> {
	static constexpr bool value = true;
};

namespace std {

	enum class SortResult {
		EQUAL, LEFT, RIGHT
	};

	template<typename Iter, typename Functor, typename T>
	Iter find_sorted_placement_binary(Iter begin, const Iter end, Functor toLeft, const T& t) {
		Iter left = begin, middle, right = end;
		while (left != right) {
			middle = left + ((right - left) / 2);
			if (toLeft(*middle, t)) {
				if (!toLeft(*(middle - 1), t)) return middle - 1;
				left = middle + 1;
			} else {
				right = middle;
			}
		}
		return left;
	}

	template<typename Iter, typename Functor, typename T>
	Iter find_binary(Iter begin, const Iter end, Functor cmp, const T& t) {
		Iter left = begin, middle, right = end;
		while (left != right) {
			middle = left + ((right - left) / 2);
			auto result = cmp(*middle, t);
			if (result == SortResult::EQUAL) {
				return middle;
			} else if (result == SortResult::LEFT) {
				left = middle + 1;
			} else {
				right = middle;
			}
		}
		return end;
	}

	template<typename... Args>
	inline bool all(Args... args) {
		return (args && ...);
	}

};

template<typename T>
class Bucket {
public:
	struct Entry {
		Entity entity;
		T val;
		Entry(Entity entity, const T& val) : entity{ entity }, val{ val } {}
		template<typename... Args>
		Entry(Entity entity, Args... args) : entity{ entity }, val{ std::forward<Args...>(args...) } {}

		inline bool operator<(Entity entity) const { return this->entity < entity; }
		inline bool operator==(Entity entity) const { return this->entity == entity; }
	};

	void insert(Entity entity, const T& t) {
		if (bucket.back().entity < entity) {
			bucket.emplace_back(entity, t);
		} else {
			auto iter = std::find_sorted_placement_binary(
				this->begin(), this->end(), std::less<void>(), entity
			);
			assert(entity == iter->entity);
			this->bucket.emplace(iter, entity, t);
		}
	}
	
	template<typename... Args>
	void emplace(Entity entity, Args... args) {
		if (bucket.empty() || bucket.back().entity < entity) {
			bucket.emplace_back(entity, args...);
		} else {
			auto iter = std::find_sorted_placement_binary(
				this->begin(), this->end(), std::less<void>(), entity
			);
			assert(entity == iter->entity);
			this->bucket.emplace(iter, entity, args...);
		}
	}

	T* get(Entity entity) {
		constexpr auto compare = [](const auto& entry, Entity entity) {
			if (entry.entity == entity) return std::SortResult::EQUAL;
			else if (entry.entity < entity) return std::SortResult::LEFT;
			else return std::SortResult::RIGHT;
		};
		auto iter = std::find_binary(
			this->begin(), this->end(),
			compare, entity
		);
		if (iter == this->end()) return nullptr;
		return &((*iter).val);
	}

	inline auto begin() { return this->bucket.begin(); }
	inline auto end() { return this->bucket.end(); }
	
	inline auto cbegin() const { return this->bucket.cbegin(); }
	inline auto cend() const { return this->bucket.cend(); }

private:
	std::vector<Entry> bucket;
public:
	using iterator = typename std::vector<Entry>::iterator;
	
	inline Entity getEntityID(const iterator& iter) {
		if (iter == this->end()) return UINT64_MAX;
		return iter->entity;
	}

	inline void advanceUntil(iterator& iter, size_t target) {
		while (iter != this->end() && iter->entity < target) iter++;
	}

};

template<typename... Contents>
class Buckets {
public:
	Buckets() {
		static_assert((is_contained<Contents, ALL_COMPONENTS_VARADIC>::value && ...), "Contents must be a component.");
		static_assert(is_unique<Contents...>::value, "Buckets Contents cannot contain duplicates.");
	}
	template<typename T>
	inline Bucket<T>& get() {
		static_assert(is_contained<T, Contents...>::value, "Cannot get type not in Contents.");
		return std::get<Bucket<T>>(buckets);
	}

	template<typename T>
	inline const Bucket<T>& get() const {
		static_assert(is_contained<T, Contents...>::value, "Cannot get type not in Contents.");
		return std::get<Bucket<T>>(buckets);
	}

	template<typename Component>
	std::vector<std::tuple<Component&>> query() {
		std::vector<std::tuple<Component&>> results;
		Bucket<Component>& bucket = this->get<Component>();
		for (auto& entry : bucket) {
			results.push_back(std::tie(entry.val));
		}
		return results;
	}

	template<typename... Components,
		std::enable_if_t<(sizeof...(Components) > 1), bool> = true>
	std::vector<std::tuple<Components&...>> query() {
		static_assert((is_contained<Components, Contents...>::value && ...), "Wack");
		std::vector<std::tuple<Components&...>> results;

		std::tuple<std::pair<Bucket<Components>&, Bucket<Components>::iterator>...> bucketIters{
			std::pair<Bucket<Components>&, Bucket<Components>::iterator>{this->get<Components>(), this->get<Components>().begin()}...
		};

		while (std::apply( // Are all iterators not at `end`.
			[](const std::pair<Bucket<Components>&, Bucket<Components>::iterator>&... bucketIter) {
				return ((bucketIter.second != bucketIter.first.end()) &&...);
			}, bucketIters
		)) {
			auto arr = std::apply(
				[](const std::pair<Bucket<Components>&, Bucket<Components>::iterator>&... bucketIter) {
					return std::array<size_t, sizeof...(Components)>({ bucketIter.first.getEntityID(bucketIter.second)... });
				}, bucketIters
			);
			const size_t max = *std::max_element(begin(arr), end(arr));

			std::apply( // Advance the iterators to the previous max.
				[max](std::pair<Bucket<Components>&, Bucket<Components>::iterator>&... bucketIter) {
					(bucketIter.first.advanceUntil(bucketIter.second, max), ...);
				}, bucketIters
			);
			if (std::apply( // Are all of the iterators referring to the same entity.
				[max](const std::pair<Bucket<Components>&, Bucket<Components>::iterator>&... bucketIter) {
					return std::all(max == bucketIter.first.getEntityID(bucketIter.second)...);
				}, bucketIters
			)) {
				std::apply( // Put the references of components into the results vector and increment the iterators.
					[&results](std::pair<Bucket<Components>&, Bucket<Components>::iterator>&... bucketIter) {
						results.push_back(std::tie(bucketIter.second->val...));
						(++bucketIter.second, ...);
					}, bucketIters
				);
			}
		}
		return results;
	}

	constexpr size_t size() const { return sizeof...(Contents); }

private:
	std::tuple<Bucket<Contents>...> buckets;
};

template<typename... Components>
class EntityConstructor {
public:
	EntityConstructor(const EntityConstructor<Components...>&) = delete;
	EntityConstructor(const EntityConstructor&&) = delete;
	
	template<typename T,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, bool> = true,
		std::enable_if_t<!is_contained<T, Components...>::value, bool> = true>
	inline EntityConstructor<T, Components...> addComponent(const T&& component) {
		this->storage.addComponent<T>(this->entity, component);
		return EntityConstructor<T, Components...>(*this);
	}
	
	template<typename T, typename... Args,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, bool> = true,
		std::enable_if_t<!is_contained<T, Components...>::value, bool> = true>
	inline EntityConstructor<T, Components...> emplaceComponent(Args... args) {
		this->storage.emplaceComponent<T, Args...>(this->entity, args...);
		return EntityConstructor<T, Components...>(*this);
	}

	inline Entity build() const { return this->entity; }

private:
	friend class EntityStorage;
	template<typename... Comps>
	friend class EntityConstructor;
	Entity entity;
	EntityStorage& storage;
	//EntityConstructor(Entity entity, EntityStorage& storage) : entity(entity), storage(storage) {}
	EntityConstructor(const EntityConstructor<>& con) : entity(con.entity), storage(con.storage) {}
	template<typename... Comps>
	EntityConstructor(const EntityConstructor<Comps...>& con) : entity(con.entity), storage(con.storage) {}
};

template<>
class EntityConstructor<> {
public:
	EntityConstructor(const EntityConstructor&) = delete;
	EntityConstructor(const EntityConstructor&&) = delete;

	template<typename T,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, bool> = true>
	inline EntityConstructor<T> addComponent(const T&& component) {
		this->storage.addComponent<T>(this->entity, component);
		return EntityConstructor<T>(*this);
	}

	template<typename T, typename... Args,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, bool> = true>
	inline EntityConstructor<T> emplaceComponent(Args... args) {
		this->storage.emplaceComponent<T, Args...>(this->entity, args...);
		return EntityConstructor<T>(*this);
	}

	inline Entity build() const { return this->entity; }

private:
	friend class EntityStorage;
	template<typename... Comps>
	friend class EntityConstructor;
	Entity entity;
	EntityStorage& storage;
	EntityConstructor(Entity entity, EntityStorage& storage) : entity(entity), storage(storage) {}
};

class EntityStorage {
public:

	inline EntityConstructor<> newEntity() {
		return EntityConstructor<>(nextID++, *this);
	}

	template<typename T,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, int> = 0>
	T& get(Entity entity) {
		auto& bucket = this->componentBuckets.get<T>();
		return *bucket.get(entity);
	}

	template<typename T,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, int> = 0>
	const T& get(Entity entity) const {
		auto& bucket = this->componentBuckets.get<T>();
		return *bucket.get(entity);
	}

	template<typename T,
		std::enable_if_t<is_contained<T, ALL_COMPONENTS_VARADIC>::value, int> = 0>
	std::optional<T*> opt(Entity entity) {
		Bucket<T>& bucket = this->componentBuckets.get<T>();
		T* component = bucket.get(entity);
		if (component) return std::optional<T*>(component);
		return std::nullopt;
	}

	template<typename... Components>
	inline std::vector<std::tuple<Components&...>> query() {
		return this->componentBuckets.query<Components...>();
	}

private:
	template<typename... Comps>
	friend class EntityConstructor;
	size_t nextID;
	Buckets<ALL_COMPONENTS_VARADIC> componentBuckets;

	template<typename T>
	inline void addComponent(Entity entity, const T& t) {
		this->componentBuckets.get<T>().insert(entity, t);
	}

	template<typename T, typename... Args>
	inline void emplaceComponent(Entity entity, Args... args) {
		this->componentBuckets.get<T>().emplace(entity, args...);
	}

};
