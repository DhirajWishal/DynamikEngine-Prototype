#pragma once

/*
 - Event System -

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 @Reference: The Cherno Project: Hazel Engine(Game engine series)
*/

#ifndef _DYNAMIK_EVENTS_BASE_H
#define _DYNAMIK_EVENTS_BASE_H

#include <string>
#include <sstream>
#include <functional>

#include "Platform.h"

#define BIT(x) (1 << x)

namespace Dynamik {
	enum class EVENT_TYPE {
		none,
		windowClose, windowResize, windowFocus, windowLostFocus, windowMoved,
		appTick, appUpdate, appRender,
		keyPressed, keyReleased, keyTyped,
		mouseButtonPressed, mouseButtonReleased, mouseMoved, mouseScrolled
	};

	enum EVENT_CATEGORY {
		None,
		eventCategoryApplication = BIT(0),
		eventCategoryInput = BIT(1),
		eventCategoryKeyboard = BIT(2),
		eventCategoryMouse = BIT(3),
		eventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EVENT_TYPE getStaticType() { return EVENT_TYPE::##type; }\
								virtual EVENT_TYPE getEventType() const override { return getStaticType(); }\
								virtual CCPTR getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	// Events Base class
	class Event {
	public:
		bool handled = false;

		virtual EVENT_TYPE getEventType() const = 0;
		virtual CCPTR getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EVENT_CATEGORY cat) {
			return getCategoryFlags() & cat;
		}

	private:
	};

	// Event Dispatcher class
	class DMK_API EventDispatcher {
		template<typename TN>
		using eventFunc = std::function<bool(TN&)>;
	public:
		EventDispatcher(Event& event) : ed_Event(event) {}

		template<typename TN>
		bool dispatch(eventFunc<TN> func);

	private:
		Event& ed_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.toString().c_str();
	}

	/* GLFW bindings */

	enum EventType {
		KEY_PRESSED_EVENT,
		KEY_RELEASED_EVENT,
		KEY_REPEAT_EVENT,
		MOUSE_BUTTON_PRESSED_EVENT,
		MOUSE_BUTTON_RELEASED_EVENT,
		MOUSE_SCROLLED_EVENT,
		MOUSE_MOVED_EVENT,
		APPLICATION_EVENT
	};

	struct DMK_API eventData {
		Event* event = nullptr;
		EventType type = {};
	};

	struct DMK_API keyEventProperties {
		int keyCode;
		int scancode;
		int action;
		int modes;
	};

	struct DMK_API mouseButtonEventProperties {
		int button;
		int action;
		int modes;
	};

	struct DMK_API mouseScrolledEventProperties {
		float posX;
		float posY;
	};

	struct DMK_API cursorPositionEventProperties {
		float posX;
		float posY;
	};

	struct DMK_API eventProperties {
		keyEventProperties keyEventProps;
		mouseButtonEventProperties mouseButtonEventProps;
		mouseScrolledEventProperties mouseScrolledProps;
		cursorPositionEventProperties mouseCursorPositionProps;
	};
}

#endif // !_DYNAMIK_EVENTS_H
