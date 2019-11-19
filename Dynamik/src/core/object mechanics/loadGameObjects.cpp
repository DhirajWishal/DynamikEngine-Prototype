#include "dmkafx.h"
#include "loadGameObjects.h"

namespace Dynamik {
	//InputContainer myContainerTmp;

	//void submit(InputContainer(*Function)()) {
	//	myContainerTmp = (*Function)();
	//}

	std::vector<GameObject> InputLoader::run() {
		return submit().assets;
	}

	std::vector<std::vector<std::string>> InputLoader::getTexturePaths() {
		std::vector<std::vector<std::string>> path;
		for (auto i : myContainer.assets) {
			path.push_back(i.getProperties().texturePaths);
		}

		return path;
	}

	std::vector<std::string> InputLoader::getModelPaths() {
		std::vector<std::string> path;
		for (auto i : myContainer.assets) {
			//path.push_back(i.getProperties().objectPath);
		}

		return path;
	}
}