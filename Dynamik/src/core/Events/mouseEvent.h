#pragma once

#include "event.h"

namespace Dynamik {

	// Moused Moved Event 
	class DMK_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) :
			m_MouseX(x), m_MouseY(y) {}

		inline float getMouseX() { return m_MouseX; }
		inline float getMouseY() { return m_MouseY; }

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(mouseMoved) EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
	};

	// Mouse Scrolled Event
	class DMK_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOff, float yOff) :
			m_XOffset(xOff), m_YOffset(yOff) {}

		inline float getMouseOffsetX() { return m_XOffset; }
		inline float getMouseOffsetY() { return m_YOffset; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(mouseScrolled) EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	// Mouse Button Event
	class DMK_API MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	// Mouse Button Presssed Event
	class DMK_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(mouseButtonPressed)
	};

	// Mouse Button Released Event
	class DMK_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(mouseButtonReleased)
	};
}