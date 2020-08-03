#pragma once
#include "XEON/events/event.h"

#define APPLICATION_EVENT_CATEGORY EVENT_CLASS_CATEGORY(EventCategory::APPLICATION | EventCategory::NONE)

namespace XEON {

	class XEON_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) { }

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent{width: " << width << ", height: " << height << "}";
			return ss.str();
		}

		APPLICATION_EVENT_CATEGORY
		EVENT_CLASS_TYPE(WINDOW_RESIZE)

	private:
		unsigned int width, height;
	};

	class XEON_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}
		APPLICATION_EVENT_CATEGORY
		EVENT_CLASS_TYPE(WINDOW_CLOSE)
	};

	class XEON_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		APPLICATION_EVENT_CATEGORY
		EVENT_CLASS_TYPE(APP_TICK)
	};

	class XEON_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		APPLICATION_EVENT_CATEGORY
		EVENT_CLASS_TYPE(APP_UPDATE)
	};

	class XEON_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		APPLICATION_EVENT_CATEGORY
		EVENT_CLASS_TYPE(APP_RENDER)
	};

}
