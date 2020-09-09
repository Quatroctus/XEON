#pragma once

#include <memory>

#ifdef _WIN32
	#define XEON_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error IOS simulator is not supported!
	#elif TARGET_OS_IPHONE == 1
		#define XEON_PLATFORM_IOS
		#error IOS is not supported!
	#elif TARGET_OS_MAC == 1
		#define XEON_PLATFORM_MAC
		#error MaxOS is not supported!
	#else 
		#error Unknown Mac platform!
	#endif
#elif defined(__ANDROID__)
	#define XEON_PLATFORM_ANDROID
	#error Android is not supported!
#elif defined(__linux__)
	#define XEON_PlATFORM_LINUX
	#error Linux is not supported!
#else
	#error Unknown Platform!
#endif

#ifdef XEON_PLATFORM_WINDOWS
	#if XEON_DYNAMIC_LINK
		#ifdef XEON_BUILD_DLL
			#define XEON_API __declspec(dllexport)
		#else
			#define XEON_API __declspec(dllimport)
		#endif
	#else
		#define XEON_API
	#endif
#else
	#error XEON only supports Windows.
#endif

#ifdef XEON_DEBUG
	#define XEON_ENABLE_ASSERTS
#endif

#ifdef XEON_ENABLE_ASSERTS
#define XEON_ASSERT(x, ...) { if (!(x)) { XEON_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#define APP_ASSERT(x, ...) { if (!(x)) { APP_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
#define XEON_ASSERT(x, ...)
#define APP_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define XEON_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace XEON {
	template<typename T> using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T> using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
