#pragma once

#include "input.h"
#include "event.h"

namespace Dynamik {
	//namespace Input {
	class inputHandler : Input {
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
