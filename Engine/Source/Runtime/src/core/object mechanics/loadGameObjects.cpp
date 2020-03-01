#include "dmkafx.h"
#include "loadGameObjects.h"

namespace Dynamik {
	//InputContainer myContainerTmp;

	//void submit(InputContainer(*Function)()) {
	//	myContainerTmp = (*Function)();
	//}

	std::vector<GameObject*> InputLoader::run() {
		return std::vector<GameObject*>();
	}

	std::vector<std::vector<std::wstring>> InputLoader::getTexturePaths() {
		std::vector<std::vector<std::wstring>> path;
		for (auto i : myContainer.assets) {
			path.push_back(i->getProperties().texturePaths);
		}

		return path;
	}

	std::vector<GameObject*> InputLoader::getRenderableGameObjects() {
		for (auto object : myContainer.assets)
			if (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT ||
				object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
				renderObjects.push_back(object);

		return renderObjects;
	}

	std::vector<std::wstring> InputLoader::getModelPaths() {
		std::vector<std::wstring> path;
		for (auto i : myContainer.assets) {
			//path.push_back(i.getProperties().objectPath);
		}

		return path;
	}
}