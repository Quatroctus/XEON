#pragma once
#include "core.h"

namespace XEON {
	class XEON_API Application {
	public:
		Application();
		virtual ~Application();
		void run();
	};

	// Defined by XEON Application.
	Application* CreateApplication();

};
