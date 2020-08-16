#pragma once
#include "XEON/input.h"
#include "XEON/application.h"

#include <GLFW/glfw3.h>

namespace XEON {

	class XEON_API WindowsInput : public Input {
	public:
		virtual inline int& getKeyRepeatCountImpl(int keyCode) override;
		virtual inline bool isKeyPressedImpl(int keyCode) const override;

		virtual inline bool isMouseButtonPressedImpl(int button) const override;

		virtual inline float getMouseXImpl() const override;
		virtual inline float getMouseYImpl() const override;

		virtual void setMousePosImpl(float x, float y) override;

	private:
		float mouseX = 0, mouseY = 0;
		std::unordered_map<int, int> repeatCounts;
	};

}
