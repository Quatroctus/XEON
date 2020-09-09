#pragma once
#ifdef XEON_PLATFORM_WINDOWS

extern XEON::Application* XEON::CreateApplication();

int main(int argc, char** argv) {
	XEON::Log::Init(nullptr);
	auto app = XEON::CreateApplication();
	app->run();
	delete app;
	return 0;
}

#endif