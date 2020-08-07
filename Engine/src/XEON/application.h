#pragma once
#include "XEON/core.h"
#include "XEON/window.h"
#include "XEON/layer_stack.h"
#include "XEON/events/application_event.h"


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
		void onEvent(Event& e);
	
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

	private:
		bool running = true;
		std::unique_ptr<Window> window;
		LayerStack layerStack;

		bool onWindowClose(WindowCloseEvent& e);

	};

	// Defined by XEON Application.
	Application* CreateApplication();

};
