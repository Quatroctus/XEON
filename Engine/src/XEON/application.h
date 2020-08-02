#pragma once
#include "core.h"

namespace XEON {

	struct ApplicationData {
		const char* loggerName = "XEON Application";
	};

	class XEON_API Application {
	public:
		ApplicationData applicationData;
		Application(ApplicationData data);
		virtual ~Application();
		void run();
	};

	// Defined by XEON Application.
	Application* CreateApplication();

};
