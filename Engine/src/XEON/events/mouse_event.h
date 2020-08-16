#pragma once

#include "XEON/events/event.h"

namespace XEON {
	class XEON_API MouseEvent : public Event {
	public:
		inline float getX() const { return mouseX; }
		inline float getY() const { return mouseY; }

		EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::NONE)

	protected:
		float mouseX, mouseY;
		MouseEvent(float mouseX, float mouseY) : mouseX(mouseX), mouseY(mouseY) {}

	};

	class XEON_API MouseMovedEvent : public MouseEvent {
	public:
		MouseMovedEvent(float mouseXLast, float mouseYLast, float mouseX, float mouseY)
			: MouseEvent(mouseX, mouseY), mouseXLast(mouseXLast), mouseYLast(mouseYLast) {}

		inline float getLastX() const { return mouseXLast; }
		inline float getLastY() const { return mouseYLast; }
		inline float getDeltaX() const { return mouseX - mouseXLast; }
		inline float getDeltaY() const { return mouseY - mouseYLast; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent{mouseX: " << mouseX << ", mouseY: " << mouseY 
				<< ", mouseXLast: " << mouseXLast << ", mouseYLast: " << mouseYLast << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOTION)

	private:
		float mouseXLast, mouseYLast;
	};

	class XEON_API MouseScrolledEvent : public MouseEvent {
	public:
		MouseScrolledEvent(float mouseX, float mouseY, float xOffset, float yOffset)
			: MouseEvent(mouseX, mouseY), xOffset(xOffset), yOffset(yOffset) {}

		inline float getXOffset() const { return xOffset; }
		inline float getYOffset() const { return yOffset; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent{mouseX: " << mouseX << ", mouseY: " << mouseY
				<< ", xOffset: " << xOffset << ", yOffset: " << yOffset << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLLED)

	private:
		float xOffset, yOffset;
	};

	class XEON_API MouseButtonEvent : public MouseEvent {
	public:
		inline int getButton() const { return button; }

		EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::MOUSE_BUTTON)
	protected:
		int button;
		MouseButtonEvent(float mouseX, float mouseY, int button) : MouseEvent(mouseX, mouseY), button(button) {}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(float mouseX, float mouseY, int button) : MouseButtonEvent(mouseX, mouseY, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent{mouseX: " << mouseX << ", mouseY: " << mouseY << ", button: " << button << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class XEON_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(float mouseX, float mouseY, int button) : MouseButtonEvent(mouseX, mouseY, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent{mouseX: " << mouseX << ", mouseY: " << mouseY << ", button: " << button << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};

}