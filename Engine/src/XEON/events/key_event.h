#pragma once

#include "XEON/events/event.h"

namespace XEON {
	class XEON_API KeyEvent : public Event {
	public:
		inline int getKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::KEYBOARD | EventCategory::INPUT)

	protected:
		const int keyCode;
		KeyEvent(int keyCode) : keyCode(keyCode) {}
	};

	class XEON_API KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return repeatCount; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent{keyCode: " << keyCode << ", repeatCount: " << repeatCount << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESSED)

	private:
		const int repeatCount;
	};

	class XEON_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent{keyCode: " << keyCode << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASED)
	};

}