#include "keyEvent.h"

namespace Dynamik {
	std::wstring KeyPressedEvent::toString() const {
		std::wstringstream ss;
		ss << "KeyPressedEvent: " << k_KeyCode <<
			" (for " << k_KeyCount << " times)";
		return ss.str();
	}

	std::wstring KeyReleasedEvent::toString() const {
		std::wstringstream ss;
		ss << "KeyReleasedEvent: " << k_KeyCode;
		return ss.str();
	}

	std::wstring KeyTypedEvent::toString() const {
		std::wstringstream ss;
		ss << "KeyTypedEvent: " << k_KeyCode;
		return ss.str();
	}
}