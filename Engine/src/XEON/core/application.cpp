#include "xeonpch.h"
#include "XEON/core/application.h"

#include "XEON/renderer/renderer.h"

#include <GLFW/glfw3.h>

namespace XEON {

	Application* Application::Instance = nullptr;

	Application::Application(ApplicationData data) : applicationData(data) {
		XEON_PROFILE_FN();
		XEON_ASSERT(!Instance, "Application already exists.");
		Instance = this;
		Log::Init(data.loggerName);
		// TODO: Provide Window Data to Window::Create.
		window = Window::Create();
		window->setEventCallback(XEON_BIND_EVENT_FN(Application::onEvent));

		Renderer::Init();

		imguiLayer = new ImGuiLayer();
		pushOverlay(imguiLayer);
	}

	Application::~Application() {
		XEON_PROFILE_FN();
	}

	void Application::run() {
		XEON_PROFILE_FN();

		while (running) {
			XEON_PROFILE_SCOPE("RunLoop");
			float time = (float) glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimized) {
				{
					XEON_PROFILE_SCOPE("LayerStack Update");
					for (Layer* layer : layerStack) layer->onUpdate(timestep);
				}
				imguiLayer->begin();
				{
					XEON_PROFILE_SCOPE("LayerStack ImGui Update");
					for (Layer* layer : layerStack) layer->onImGuiRender();
				}
				imguiLayer->end();
			}


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
		XEON_PROFILE_FN();
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		XEON_PROFILE_FN();
		layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		XEON_PROFILE_FN();

		if (e.getWidth() == 0 || e.getHeight() == 0) {
			minimized = true;
			return false;
		}
		minimized = false;
		Renderer::OnWindowResize(e.getWidth(), e.getHeight());
		return false;
	}

}
