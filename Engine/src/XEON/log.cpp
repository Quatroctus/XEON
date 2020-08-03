#include "xeonpch.h"
#include "XEON/log.h"
#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

namespace XEON {
	
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::appLogger;

	void Log::Init(const char* name) {
		spdlog::set_pattern("%^[%T %n]: %v%$");
		coreLogger = spdlog::stdout_color_mt("XEON");
		coreLogger->set_level(spdlog::level::trace);
		appLogger = spdlog::stdout_color_mt(name);
		appLogger->set_level(spdlog::level::trace);
	}

}