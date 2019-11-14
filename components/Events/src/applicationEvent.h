#pragma once

#ifndef _DYNAMIK_EVENTS_APPLICATION_EVENTS_H
#define _DYNAMIK_EVENTS_APPLICATION_EVENTS_H

#include "event.h"

namespace Dynamik {
	// Window Resize Event (Base class)
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :
			w_Width(width), w_Height(height) {}

		inline unsigned int getWidth() const { return w_Width; }
		inline unsigned int getHeight() const { return w_Height; }

		std::string toString() const override;

		EVENT_CLASS_TYPE(windowResize) EVENT_CLASS_CATEGORY(eventCategoryApplication)

	private:
		unsigned int w_Height, w_Width;
	};

	// Window Close Event Class
	class  WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(windowClose) EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	// App Tick Event
	class  AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(appTick) EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	// App Update Event
	class  AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(appUpdate) EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};

	// App Render Event
	class  AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(appRender) EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
}

#endif // !_DYNAMIK_EVENTS_APPLICATION_EVENTS_H
