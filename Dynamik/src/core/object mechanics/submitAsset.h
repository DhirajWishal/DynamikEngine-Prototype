#pragma once

#ifndef _DYNAMIK_SUBMIT_ASSETS_H
#define _DYNAMIK_SUBMIT_ASSETS_H

#include <vector>
#include "GameObject.h"
#include "KeyBingings.h"

namespace Dynamik {
	struct InputContainer {
		std::vector<GameObject*> assets;
		KeyBindings keyBindings;

		void addToAssetsQueue(GameObject obj) {
			assets.push_back(&obj);
		}
	};

	//InputContainer submit() {
	//	return InputContainer();
	//};
}

#endif // !_DYNAMIK_SUBMIT_ASSETS_H
