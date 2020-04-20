#include "dmkafx.h"
#include "Application.h"

#include "Engines/ADGR/StartupRenderer/StartupRenderer.h"

namespace Dynamik {	
	void Application::initializeInstance(DMKInstanceDescriptor descriptor)
	{
		ADGR::StartupRenderer::initialize();
		ADGR::StartupRenderer::initializeShaders();
		ADGR::StartupRenderer::initializeVertexBuffers();
		ADGR::StartupRenderer::loadTexure("E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg");
		instance();
	}

	UI32 Application::addLevel(DMKLevelDescriptor level)
	{
		ARRAY<ARRAY<AssetContainer>> scenes;
		for (auto scene : level.scenes)
		{
			ARRAY<AssetContainer> assets;
			for (auto object : scene.assets)
			{
				AssetContainer _container;
				_container.address = object;
				_container.byteSize = sizeof(*object);
				_container.type = object->type;
				assets.pushBack(_container);
			}
			scenes.pushBack(assets);
		}

		return instance().myAssetManager.addLevel(scenes);
	}

	void Application::loadLevel(UI32 levelIndex)
	{
		instance().levelIndex = levelIndex;
	}

	void Application::loadScene(UI32 sceneIndex)
	{
		instance().sceneIndex = sceneIndex;
		instance().myAssetManager.loadScene(sceneIndex, instance().levelIndex);
	}

	UI32 Application::addAsset(DMKGameObject* object)
	{
		AssetContainer _container;
		_container.address = object;
		_container.byteSize = sizeof(*object);
		_container.type = object->type;
		
		return instance().myAssetManager.addAsset(_container, 0, 0);
	}
	
	void Application::initializeRenderingEngine()
	{
	}
	
	Application& Application::instance()
	{
		static Application myInstance;
		return myInstance;
	}
}