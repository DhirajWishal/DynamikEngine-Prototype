#pragma once

#include <vector>
#include "GameObject/GameObject.h"
#include "Key Bindings/KeyBingings.h"

namespace Dynamik {
	struct InputContainer {
		std::vector<GameObject> assets;
		KeyBindings keyBindings;

		void addToAssetsQueue(GameObject& obj) {
			assets.push_back(obj);
		}
	};

	InputContainer submit();
}
