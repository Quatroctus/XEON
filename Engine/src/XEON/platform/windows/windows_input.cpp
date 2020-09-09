#include "xeonpch.h"
#include "XEON/platform/windows/windows_input.h"

namespace XEON {
	
	Scope<Input> Input::input = CreateScope<WindowsInput>();

	int& WindowsInput::getKeyRepeatCountImpl(int keyCode) {
		return repeatCounts.find(keyCode) != repeatCounts.end() ? repeatCounts.find(keyCode)->second : (repeatCounts[keyCode] = -1);
	}

	bool WindowsInput::isKeyPressedImpl(int keyCode) const {
		return glfwGetKey(static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow()), keyCode);
	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) const {
		return glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow()), button);
	}

	float WindowsInput::getMouseXImpl() const { return mouseX; }

	float WindowsInput::getMouseYImpl() const { return mouseY; }

	void WindowsInput::setMousePosImpl(float x, float y) { mouseX = x; mouseY = y; }

}
