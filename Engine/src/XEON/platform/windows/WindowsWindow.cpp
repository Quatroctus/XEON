#include "xeonpch.h"
#include "XEON/platform/windows/WindowsWindow.h"
#include "XEON/events/application_event.h"
#include "XEON/events/key_event.h"
#include "XEON/events/mouse_event.h"
#include "XEON/input.h"

#include "glad/glad.h"

namespace XEON {
	
	static bool GLFW_Initialized = false;

	static void GLFWErrorCallback(int error, const char* desc) {
		XEON_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	Window* Window::Create(const WindowData& data) {
		return new WindowsWindow(data);
	}

	WindowsWindow::WindowsWindow(const WindowData& data) {
		init(data);
	}

	WindowsWindow::~WindowsWindow() { }

	void WindowsWindow::init(const WindowData& data) {
		this->data.title = data.title;
		this->data.width = data.width;
		this->data.height = data.height;

		if (!GLFW_Initialized) {
			int success = glfwInit();
			XEON_ASSERT(success, "Could not initialize GLFW!");

			GLFW_Initialized = true;
		}

		window = glfwCreateWindow((int)data.width, data.height, data.title, nullptr, nullptr);
		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		XEON_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(window, &this->data);
		setVSync(true);

		glfwSetErrorCallback(GLFWErrorCallback);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data =*((WindowData*)glfwGetWindowUserPointer(window));
			WindowResizeEvent event(width, height);
			data.width = width;
			data.height = height;
			data.eventCallback(event);
		});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
			data.eventCallback(WindowCloseEvent());
		});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
			switch (action) {
				case GLFW_PRESS:
				{
					Input::setKeyRepeatCount(key, 0);
					KeyPressEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					Input::setKeyRepeatCount(key, -1);
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					int repeatCount = Input::getKeyRepeatCount(key) + 1;
					Input::setKeyRepeatCount(key, repeatCount);
					KeyPressEvent event(key, repeatCount);
					data.eventCallback(event);
					break;
				}
			}
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
			switch (action) {
				case GLFW_PRESS:
				{
					Input::setMouseButtonState(button, true);
					MouseButtonPressedEvent event(Input::getMouseX(), Input::getMouseY(), button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					Input::setMouseButtonState(button, false);
					MouseButtonReleasedEvent event(0, 0, button);
					data.eventCallback(event);
					break;
				}
			}
		});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
			MouseScrolledEvent event(Input::getMouseX(), Input::getMouseY(), (float) xOffset, (float) yOffset);
			data.eventCallback(event);
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
			MouseMovedEvent event(Input::getMouseX(), Input::getMouseY(), (int) x, (int) y);
			Input::setMousePosition(x, y);
			data.eventCallback(event);
		});
	}

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::setVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		data.isVSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return data.isVSync;
	}

}
