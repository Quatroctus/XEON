#include "xeonpch.h"
#include "XEON/application.h"

namespace XEON {
	
	Application::Application(ApplicationData data) : applicationData(data) { }

	Application::~Application() { }

	void Application::run() {
		while (true);
	}

}
