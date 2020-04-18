#include "dmkafx.h"
#include "mouseEvent.h"

namespace Dynamik {
	std::string MouseMovedEvent::toString() const {
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	std::string MouseScrolledEvent::toString() const {
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

	std::string MouseButtonPressedEvent::toString() const {
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	std::string MouseButtonReleasedEvent::toString() const {
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}
}