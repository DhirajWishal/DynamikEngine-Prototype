#pragma once

#include "event.h"

namespace Dynamik {

	// Key Event (Base class)
	class  KeyEvent : public Event {
	public:
		inline int getkeyCode() const { return k_KeyCode; }

		EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)

	protected:
		KeyEvent(int keyCode) :
			k_KeyCode(keyCode) {}

		int k_KeyCode;
	};

	class  KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repCount) : 
			KeyEvent(keyCode), k_KeyCount(repCount) {}

		inline int getRepCount() const { return k_KeyCount; }

		std::string toString() const override;

		EVENT_CLASS_TYPE(keyPressed)

	private:
		int k_KeyCount;
	};

	class  KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) :
			KeyEvent(keyCode) {}

		std::string toString() const override;

		EVENT_CLASS_TYPE(keyReleased)
	};

	class  KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode) :
			KeyEvent(keyCode) {}

		std::string toString() const override;

		EVENT_CLASS_TYPE(keyTyped)
	};

	struct keyEventData {
		KeyPressedEvent keyPressedEvent;
		KeyReleasedEvent keyReleasedEvent;
		KeyPressedEvent keyRepeatEvent;
	};
}