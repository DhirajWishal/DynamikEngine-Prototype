#pragma once

#ifndef _DYNAMIK_EVENTS_INPUTS_H
#define _DYNAMIK_EVENTS_INPUTS_H

namespace Dynamik {
	class Input {
	public:
		virtual bool isKeyPressed() = 0;
		virtual bool isKeyReleased() = 0;
		virtual bool isKeyRepeated() = 0;

		virtual int getPressedKeycode() = 0;
		virtual int getReleasedKeycode() = 0;
		//virtual int getKeycode() = 0;

	protected:
		//virtual bool isKeyPressedImpl(int keyCode) = 0;

	private:
		//static Input* myInput;
	};
}

#endif // !_DYNAMIK_EVENTS_INPUTS_H
