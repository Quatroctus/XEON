#pragma once
#include "XEON/core/core.h"
#include "XEON/core/window.h"
#include "XEON/core/layer_stack.h"
#include "XEON/imgui/imgui_layer.h"
#include "XEON/events/application_event.h"
#include "XEON/core/timestep.h"

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

		inline Window& getWindow() { return *window; }

		static inline Application& Get() { return *Instance; }

	private:
		bool running = true;
		bool minimized = false;
		std::unique_ptr<Window> window;
		ImGuiLayer* imguiLayer;
		LayerStack layerStack;
		float lastFrameTime = 0;
		Timestep timestep;

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		static Application* Instance;

	};

	// Defined by XEON Application.
	Application* CreateApplication();

};
