#pragma once

namespace Dynamik {

	class Input {
	public:
		static bool isKeyPressed(int keyCode) {
			return myInput->isKeyPressedImpl(keyCode);
		}

	protected:
		virtual bool isKeyPressedImpl(int keyCode) = 0;

	private:
		static Input* myInput;

	};
}

