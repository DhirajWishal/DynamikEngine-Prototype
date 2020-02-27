#pragma once

#ifndef _DYNAMIK_EVENTS_INPUT_HANDLER_H
#define _DYNAMIK_EVENTS_INPUT_HANDLER_H

#include "input.h"
#include "event.h"

namespace Dynamik {
	//namespace Input {
	class DMK_API inputHandler : Input {
	public:
		inputHandler(Event& event) : event(event) {}
		~inputHandler() {}

		bool isKeyPressed() override;
		bool isKeyReleased() override;
		bool isKeyRepeated() override;

		int getPressedKeycode() override;
		int getReleasedKeycode() override;

	private:
		Event& event;
	};

	//}
}

#endif // !_DYNAMIK_EVENTS_INPUT_HANDLER_H
