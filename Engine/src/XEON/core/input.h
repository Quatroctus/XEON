#pragma once

#include "XEON/core/core.h"

namespace XEON {

	// TODO: Consider controller input.
	class XEON_API Input {
	public:
		
//		inline static void setKeyRepeatCount(int keyCode, int repeatCount) { input->setKeyRepeatCountImpl(keyCode, repeatCount); }
		inline static int& getKeyRepeatCount(int keyCode) { return input->getKeyRepeatCountImpl(keyCode); }
		inline static bool isKeyPressed(int keyCode) { return input->isKeyPressedImpl(keyCode); }

//		inline static void setMouseButtonState(int button, bool state) { input->setMouseButtonStateImpl(button, state); }
		inline static bool isMouseButtonPressed(int button) { return input->isMouseButtonPressedImpl(button); }

		inline static void setMousePos(float  x, float y) { input->setMousePosImpl(x, y); }
		inline static float getMouseX() { return input->getMouseXImpl(); }
		inline static float getMouseY() { return input->getMouseYImpl(); }

	protected:
//		virtual inline void setKeyRepeatCountImpl(int keyCode, int repeatCount) = 0;
		virtual inline int& getKeyRepeatCountImpl(int keyCode) = 0;
		virtual inline bool isKeyPressedImpl(int keyCode) const = 0;

//		virtual inline void setMouseButtonStateImpl(int button, bool state) = 0;
		virtual inline bool isMouseButtonPressedImpl(int button) const = 0;

		virtual inline float getMouseXImpl() const = 0;
		virtual inline float getMouseYImpl() const = 0;
//		virtual inline void setMouseXImpl(int x) = 0;
//		virtual inline void setMouseYImpl(int y) = 0;
		virtual inline void setMousePosImpl(float x, float y) = 0;

	private:
		static Scope<Input> input;
	};

}
