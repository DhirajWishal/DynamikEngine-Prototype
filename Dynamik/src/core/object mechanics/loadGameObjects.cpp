#include "dmkafx.h"
#include "loadGameObjects.h"

namespace Dynamik {

	//InputContainer myContainerTmp;

	//void submit(InputContainer(*Function)()) {
	//	myContainerTmp = (*Function)();
	//}

	void InputLoader::run() {
		myContainer = submit();
	}

	std::vector<std::string> InputLoader::getTexturePaths() {
		std::vector<std::string> path;
		for (auto i : myContainer.assets) {
			path.push_back(i.getProperties().texPath);
		}

		return path;
	}

	std::vector<std::string> InputLoader::getModelPaths() {
		std::vector<std::string> path;
		for (auto i : myContainer.assets) {
			path.push_back(i.getProperties().objPath);
		}

		return path;
	}
}
