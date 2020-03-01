#include "mouseEvent.h"

namespace Dynamik {
	std::wstring MouseMovedEvent::toString() const {
		std::wstringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	std::wstring MouseScrolledEvent::toString() const {
		std::wstringstream ss;
		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

	std::wstring MouseButtonPressedEvent::toString() const {
		std::wstringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	std::wstring MouseButtonReleasedEvent::toString() const {
		std::wstringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}
}