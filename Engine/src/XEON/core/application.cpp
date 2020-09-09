#include "xeonpch.h"
#include "XEON/core/application.h"

#include "XEON/renderer/renderer.h"

#include <GLFW/glfw3.h>

namespace XEON {

	Application* Application::Instance = nullptr;

	Application::Application(ApplicationData data) : applicationData(data) {
		XEON_ASSERT(!Instance, "Application already exists.");
		Instance = this;
		Log::Init(data.loggerName);
		// TODO: Provide Window Data to Window::Create.
		window = Window::Create();
		window->setEventCallback(XEON_BIND_EVENT_FN(Application::onEvent));
		window->setVSync(false);

		Renderer::Init();

		imguiLayer = new ImGuiLayer();
		pushOverlay(imguiLayer);
	}

	Application::~Application() { }

	void Application::run() {
		while (running) {
			float time = (float) glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimized) {
				for (Layer* layer : layerStack) layer->onUpdate(timestep);
			}

			imguiLayer->begin();
			for (Layer* layer : layerStack) layer->onImGuiRender();
			imguiLayer->end();

			window->onUpdate();
		}
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(XEON_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(XEON_BIND_EVENT_FN(Application::onWindowResize));
		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.handled) break;
		}
	}

	void Application::pushLayer(Layer* layer) {
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay) {
		layerStack.pushOverlay(overlay);
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		if (e.getWidth() == 0 || e.getHeight() == 0) {
			minimized = true;
			return false;
		}
		minimized = false;
		Renderer::OnWindowResize(e.getWidth(), e.getHeight());
		return false;
	}

}
