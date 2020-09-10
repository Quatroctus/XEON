#pragma once

#include "XEON/debug/instrumentor.h"

#ifdef XEON_PLATFORM_WINDOWS

extern XEON::Application* XEON::CreateApplication();

int main(int argc, char** argv) {
	XEON::Log::Init(nullptr);
	XEON_PROFILE_BEGIN_SESSION("Startup", "XEON_Profile-Startup.json");
	auto app = XEON::CreateApplication();
	XEON_PROFILE_BEGIN_NEW_SESSION("Runtime", "XEON_Profile-Runtime.json");
	app->run();
	XEON_PROFILE_BEGIN_NEW_SESSION("Shutdown", "XEON_Profile-Shutdown.json");
	delete app;
	XEON_PROFILE_END_SESSION();
	return 0;
}

#endif