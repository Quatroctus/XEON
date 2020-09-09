#include "xeonpch.h"
#include "XEON/core/log.h"
#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

namespace XEON {
	
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::appLogger;

	void Log::Init(const char* name) {
		spdlog::set_pattern("%^[%T %n]: %v%$");
		if (!coreLogger){
			coreLogger = spdlog::stdout_color_mt("XEON");
			coreLogger->set_level(spdlog::level::trace);
		}
		if (appLogger && appLogger->name() == "APP" && !name) {
			appLogger.reset();
			appLogger = spdlog::stdout_color_mt(name);
			appLogger->set_level(spdlog::level::trace);
		} else if (!appLogger) {
			appLogger = spdlog::stdout_color_mt(name ? name : "APP");
			appLogger->set_level(spdlog::level::trace);
		}
	}



}