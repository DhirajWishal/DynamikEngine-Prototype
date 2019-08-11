#pragma once

/*
 - Event System -

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 @Reference: The Cherno Project: Hazel Engine(Game engine series)
*/

#include <string>
#include <sstream>
#include <functional>

#include "core/core.h"

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
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	// Events Base class
	class Event {
	public:
		bool handled = false;

		virtual EVENT_TYPE getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EVENT_CATEGORY cat) {
			return getCategoryFlags() & cat;
		}

	private:

	};


	// Event Dispatcher class
	class EventDispatcher {
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
		return os << e.toString();
	}

}
