#pragma once

#include "XEON/core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(push, 0)

namespace XEON {
	class XEON_API Log {
	public:
		static void Init(const char* name);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return appLogger; };
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> appLogger;
	};
}

#pragma warning(pop)
// XEON Logging MACROS
#define XEON_TRACE(...) ::XEON::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define XEON_INFO(...) ::XEON::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XEON_WARN(...) ::XEON::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XEON_ERROR(...) ::XEON::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XEON_FATAL(...) ::XEON::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Application Logging MACROS
#define APP_TRACE(...) ::XEON::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_INFO(...) ::XEON::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...) ::XEON::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...) ::XEON::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_FATAL(...) ::XEON::Log::GetAppLogger()->critical(__VA_ARGS__)
