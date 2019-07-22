#pragma once

#include "event.h"

namespace Dynamik {

	// Key Event (Base class)
	class DMK_API KeyEvent : public Event {
	public:
		inline int getkeyCode() const { return k_KeyCode; }

		EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)

	protected:
		KeyEvent(int keyCode) :
			k_KeyCode(keyCode) {}

		int k_KeyCode;
	};

	class DMK_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repCount) : 
			KeyEvent(keyCode), k_KeyCount(repCount) {}

		inline int getRepCount() const { return k_KeyCount; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << k_KeyCode <<
				" (for " << k_KeyCount << " times)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(keyPressed)

	private:
		int k_KeyCount;
	};

	class DMK_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) :
			KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << k_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(keyReleased)
	};

	class DMK_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode) :
			KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << k_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(keyTyped)
	};
}