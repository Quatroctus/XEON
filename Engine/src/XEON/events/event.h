#pragma once

#include "xeonpch.h"
#include "XEON/core/core.h"

namespace XEON {

	enum class EventType {
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
		APP_TICK, APP_UPDATE, APP_RENDER,
		KEY_PRESSED, KEY_TYPED, KEY_RELEASED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOTION, MOUSE_SCROLLED
	};

	enum class EventCategory {
		NONE = 0,
		APPLICATION = BIT(0),
		INPUT = BIT(1),
		KEYBOARD = BIT(2),
		MOUSE = BIT(3),
		MOUSE_BUTTON = BIT(4)
	};
	inline int operator& (int value, EventCategory category) { return value & static_cast<int>(category); }
	inline int operator| (EventCategory c1, EventCategory c2) { return static_cast<int>(c1) | static_cast<int>(c2); }


#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class XEON_API Event {
		friend class EventDispatcher;
	public:
		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; }
	};

	class EventDispatcher {
		
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : event(event) { }

		template<typename T>
		bool dispatch(EventFn<T> func) {
			if (event.getEventType() == T::getStaticType()) {
				event.handled = func(*(T*)&event);
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return (os << e.toString()); }

}
