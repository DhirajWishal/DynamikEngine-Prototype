#include "dmkafx.h"
#include "loadGameObjects.h"

namespace Dynamik {
	//InputContainer myContainerTmp;

	//void submit(InputContainer(*Function)()) {
	//	myContainerTmp = (*Function)();
	//}

	ARRAY<GameObject*> InputLoader::run() {
		return ARRAY<GameObject*>();
	}

	ARRAY<ARRAY<std::string>> InputLoader::getTexturePaths() {
		ARRAY<ARRAY<std::string>> path;
		for (auto i : myContainer.assets) {
			path.push_back(i->getProperties().texturePaths);
		}

		return path;
	}

	ARRAY<GameObject*> InputLoader::getRenderableGameObjects() {
		for (auto object : myContainer.assets)
			if (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT ||
				object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
				renderObjects.push_back(object);

		return renderObjects;
	}

	ARRAY<std::string> InputLoader::getModelPaths() {
		ARRAY<std::string> path;
		for (auto i : myContainer.assets) {
			//path.push_back(i.getProperties().objectPath);
		}

		return path;
	}
}