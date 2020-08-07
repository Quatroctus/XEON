#pragma once
#include "core.h"
#include "XEON/events/event.h"

namespace XEON {

	using uint = unsigned int;
	using EventCallbackFn = std::function<void(Event&)>;

	class XEON_API Window {
	public:
		struct XEON_API WindowData {
			const char* title;
			uint width, height;
			bool isVSync;
			EventCallbackFn eventCallback;

			WindowData(
				const char* title = "XEON Application", uint width = 720, uint height = 405,
				bool vSync = false, EventCallbackFn eventCallback = nullptr
			)
				: title(title), width(width), height(height), isVSync(vSync), eventCallback(eventCallback) {}
		};

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		static Window* Create(const WindowData& data=WindowData());
	};
}