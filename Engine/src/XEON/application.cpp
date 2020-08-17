#include "xeonpch.h"
#include "XEON/application.h"
#include "input.h"
#include <glad/glad.h>

namespace XEON {
	
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::Instance = nullptr;

	Application::Application(ApplicationData data)
		: applicationData(data), window(Window::Create()) {
		XEON_ASSERT(!Instance, "Application already exists.");
		Instance = this;
		window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
		imguiLayer = new ImGuiLayer();
		pushOverlay(imguiLayer);
	}

	Application::~Application() { }

	void Application::run() {
		glClearColor(0.1F, 0.1F, 0.1F, 1);
		while (running) {
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : layerStack) layer->onUpdate();

			imguiLayer->begin();
			for (Layer* layer : layerStack) layer->onImGuiRender();
			imguiLayer->end();

			window->onUpdate();
		}
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.handled) break;
		}
	}

	void Application::pushLayer(Layer* layer) {
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		running = false;
		return true;
	}

}
