#pragma once

#include <XEON/ecs/component.h>

//template<typename T>
//concept System = requires(T t, EntityStorage& storage) {
//	{ t } -> std::invocable<T>;
//	{ t.cacheQuery(storage) };
//	{ t.lastQuery() } -> std::convertible_to< std::chrono::high_resolution_clock::time_point>;
//};

#define ENGINE_DEFINED_COMPONENTS PositionComponent, NameComponent
#ifndef USER_DEFINED_COMPONENTS
#define USER_DEFINED_COMPONENTS
#endif
#define ALL_COMPONENTS_VARADIC ENGINE_DEFINED_COMPONENTS USER_DEFINED_COMPONENTS
