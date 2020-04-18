#include "dmkafx.h"
#include "keyEvent.h"

namespace Dynamik {
	std::string KeyPressedEvent::toString() const {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << k_KeyCode <<
			" (for " << k_KeyCount << " times)";
		return ss.str();
	}

	std::string KeyReleasedEvent::toString() const {
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << k_KeyCode;
		return ss.str();
	}

	std::string KeyTypedEvent::toString() const {
		std::stringstream ss;
		ss << "KeyTypedEvent: " << k_KeyCode;
		return ss.str();
	}
}