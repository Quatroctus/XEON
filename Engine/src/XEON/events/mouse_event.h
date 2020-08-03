#pragma once

#include "XEON/events/event.h"

namespace XEON {
	class XEON_API MouseEvent : public Event {
	public:
		inline int getX() const { return mouseX; }
		inline int getY() const { return mouseY; }

		EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::NONE)

	protected:
		int mouseX, mouseY;
		MouseEvent(int mouseX, int mouseY) : mouseX(mouseX), mouseY(mouseY) {}

	};

	class XEON_API MouseMovedEvent : public MouseEvent {
	public:
		MouseMovedEvent(int mouseXLast, int mouseYLast, int mouseX, int mouseY)
			: MouseEvent(mouseX, mouseY), mouseXLast(mouseXLast), mouseYLast(mouseYLast) {}

		inline int getLastX() const { return mouseXLast; }
		inline int getLastY() const { return mouseYLast; }
		inline int getDeltaX() const { return mouseX - mouseXLast; }
		inline int getDeltaY() const { return mouseY - mouseYLast; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent{mouseX: " << mouseX << ", mouseY: " << mouseY 
				<< ", mouseXLast: " << mouseXLast << ", mouseYLast: " << mouseYLast << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOTION)

	private:
		int mouseXLast, mouseYLast;
	};

	class MouseScrolledEvent : public MouseEvent {
	public:
		MouseScrolledEvent(int mouseX, int mouseY, float xOffset, float yOffset)
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

	class MouseButtonEvent : public MouseEvent {
	public:
		inline int getButton() const { return button; }

		EVENT_CLASS_CATEGORY(EventCategory::MOUSE | EventCategory::MOUSE_BUTTON)
	protected:
		int button;
		MouseButtonEvent(int mouseX, int mouseY, int button) : MouseEvent(mouseX, mouseY), button(button) {}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int mouseX, int mouseY, int button) : MouseButtonEvent(mouseX, mouseY, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent{mouseX: " << mouseX << ", mouseY: " << mouseY << ", button: " << button << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int mouseX, int mouseY, int button) : MouseButtonEvent(mouseX, mouseY, button) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent{mouseX: " << mouseX << ", mouseY: " << mouseY << ", button: " << button << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};

}