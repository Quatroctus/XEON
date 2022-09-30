#pragma once

#include <array>

template<typename Key, typename Value, Value Default, size_t Size>
class constexpr_map {
public:
	constexpr constexpr_map() : data() {}
	constexpr constexpr_map(const std::array<std::pair<Key, Value>, Size>& data) : data(data) {}

	constexpr Value at(const Key& key) const {
		const auto iter = std::find_if(
			std::begin(data), std::end(data),
			[&key](const auto& [_key, _value]) { return _key == key; }
		);
		if (iter != std::end(data)) {
			return iter->second;
		} else {
			return Default;
		}
	}

private:
	std::array<std::pair<Key, Value>, Size> data;
};
