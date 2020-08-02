#pragma once
#ifdef XEON_PLATFORM_WINDOWS

extern XEON::Application* XEON::CreateApplication();

int main(int argc, char** argv) {
	auto app = XEON::CreateApplication();
	XEON::Log::Init(app->applicationData.loggerName);
	app->run();
	delete app;
	return 0;
}

#endif