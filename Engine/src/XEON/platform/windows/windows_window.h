#pragma once

#include "XEON/window.h"
#include <GLFW/glfw3.h>

namespace XEON {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowData& data);
		virtual ~WindowsWindow();

		void onUpdate() override;

		inline uint getWidth() const override { return data.width; }
		inline uint getHeight() const override { return data.height; }

		inline void setEventCallback(const EventCallbackFn& callback) { data.eventCallback = callback; }

		void setVSync(bool enabled) override;
		bool isVSync() const override;

		virtual inline void* getNativeWindow() const override { return window; }

	private:
		GLFWwindow* window;
		WindowData data;
		
		virtual void init(const WindowData& data);
		virtual void shutdown();
	};

}
