#pragma once

#ifdef XEON_PLATFORM_WINDOWS
#ifdef XEON_BUILD_DLL
#define XEON_API __declspec(dllexport)
#else
#define XEON_API __declspec(dllimport)
#endif
#else
#error XEON only supports Windows.
#endif

#ifdef XEON_DEBUG
#define XEON_ASSERT(x, ...) { if (!(x)) { XEON_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#define APP_ASSERT(x, ...) { if (!(x)) { APP_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
#define XEON_ASSERT(x, ...)
#define APP_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)