#include "xeonpch.h"

#include "XEON/platform/windows/windows_window.h"

#include "XEON/core/input.h"

#include "XEON/events/application_event.h"
#include "XEON/events/key_event.h"
#include "XEON/events/mouse_event.h"

#include "XEON/platform/opengl/opengl_context.h"

namespace XEON {
	
	static bool GLFW_Initialized = false;

	static void GLFWErrorCallback(int error, const char* desc) {
		XEON_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	std::unique_ptr<Window> Window::Create(const WindowData& data) {
		return std::make_unique<WindowsWindow>(data);
	}

	WindowsWindow::WindowsWindow(const WindowData& data) {
		XEON_PROFILE_FN();
		init(data);
	}

	WindowsWindow::~WindowsWindow() { 
		XEON_PROFILE_FN();
		shutdown();
	}

	void WindowsWindow::init(const WindowData& data) {
		XEON_PROFILE_FN();

		this->data.title = data.title;
		this->data.width = data.width;
		this->data.height = data.height;

		if (!GLFW_Initialized) {
			XEON_PROFILE_SCOPE("glfwInit - WindowsWindow");
			int success = glfwInit();
			XEON_ASSERT(success, "Could not initialize GLFW!");

			GLFW_Initialized = true;
		}
		{
			XEON_PROFILE_SCOPE("glfwCreateWindow - WindowsWindow");
			window = glfwCreateWindow((int)data.width, data.height, data.title, nullptr, nullptr);
		}
		context = CreateScope<OpenGLContext>(window);
		context->init();

		glfwSetWindowUserPointer(window, &this->data);
		setVSync(data.isVSync);
		{
			XEON_PROFILE_SCOPE("Setting GLFW Callbacks - WindowsWindow");
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
						Input::getKeyRepeatCount(key) = 0;
						KeyPressedEvent event(key, 0);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						Input::getKeyRepeatCount(key) = -1;
						KeyReleasedEvent event(key);
						data.eventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, ++Input::getKeyRepeatCount(key));
						data.eventCallback(event);
						break;
					}
				}
			});
			glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int key) {
				WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
				KeyTypedEvent event(key);
				data.eventCallback(event);
			});
			glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *((WindowData*)glfwGetWindowUserPointer(window));
				switch (action) {
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(Input::getMouseX(), Input::getMouseY(), button);
						data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(Input::getMouseX(), Input::getMouseY(), button);
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
				MouseMovedEvent event(Input::getMouseX(), Input::getMouseY(), (float) x, (float) y);
				Input::setMousePos((float) x, (float) y);
				data.eventCallback(event);
			});
		}
	}

	void WindowsWindow::shutdown() {
		XEON_PROFILE_FN();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::onUpdate() {
		XEON_PROFILE_FN();

		glfwPollEvents();
		context->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled) {
		XEON_PROFILE_FN();

		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		data.isVSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return data.isVSync;
	}

}
