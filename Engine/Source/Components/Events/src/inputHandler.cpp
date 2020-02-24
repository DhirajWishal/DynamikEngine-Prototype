#include "inputHandler.h"

#include "keyEvent.h"

namespace Dynamik {
	//namespace Input {
	bool inputHandler::isKeyPressed() {
		return event.getEventType() == EVENT_TYPE::keyPressed;
	}

	bool inputHandler::isKeyReleased() {
		return event.getEventType() == EVENT_TYPE::keyReleased;
	}

	bool inputHandler::isKeyRepeated() {
		return event.getEventType() == EVENT_TYPE::keyTyped;
	}

	int inputHandler::getPressedKeycode() {
		KeyPressedEvent kpe = dynamic_cast<KeyPressedEvent&>(event);

		return kpe.getkeyCode();
	}

	int inputHandler::getReleasedKeycode() {
		KeyReleasedEvent kre = dynamic_cast<KeyReleasedEvent&>(event);

		return kre.getkeyCode();
	}
	//}
}