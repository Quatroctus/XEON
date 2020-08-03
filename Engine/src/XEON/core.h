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

#define BIT(x) (1 << x)