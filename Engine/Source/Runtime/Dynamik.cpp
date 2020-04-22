/*
 Dynamik Engine API source file.

 Author:	Dhiraj Wishal
 Date:		12/04/2020
 IDE:		MS Visual Studio Community 2019

 This file contains all the necessary definitions for the Dynamik Engine API.
*/

#include "dmkafx.h"
#include "Dynamik.h"

#include "Engines/ADGR/StartupRenderer/StartupRenderer.h"

#include "Events/EventManager.h"

namespace Dynamik {
	/* Dynamik instance */
	DMKEngine DMKEngine::instance;

	void DMKEngine::initializeInstance(DMKInstanceDescriptor descriptor)
	{
		/* Initialize the startup renderer to render basic data while the main renderer is activated.
		   This way the user is not presented with a blank screen while all the scene data are loaded. */
		ADGR::StartupRenderer::initialize();
		ADGR::StartupRenderer::initializeShaders();
		ADGR::StartupRenderer::initializeVertexBuffers();
		ADGR::StartupRenderer::loadTexure("E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg");
		instance;
	}

	UI32 DMKEngine::addLevel(DMKLevelDescriptor level)
	{
		/* Since each level consists of one or more scenes, we pack the given scenes to a wrapper.	*/
		/* After wrapping them in scene wrappers, we add the assets to it.							*/
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

		/* Add the scenes to a new level and return its index. */
		return instance.myAssetManager.addLevel(scenes);
	}

	void DMKEngine::loadLevel(UI32 levelIndex)
	{
		instance.levelIndex = levelIndex;
	}

	void DMKEngine::loadScene(UI32 sceneIndex)
	{
		instance.sceneIndex = sceneIndex;
		instance.myAssetManager.loadScene(sceneIndex, instance.levelIndex);
	}

	UI32 DMKEngine::addAsset(DMKGameObject* object)
	{
		/* Create a new Asset Container for the new object. */
		AssetContainer _container;
		_container.address = object;
		_container.byteSize = sizeof(*object);
		_container.type = object->type;

		/* Add the asset to the Asset Manager and return its index. */
		return instance.myAssetManager.addAsset(_container, instance.sceneIndex, instance.levelIndex);
	}

	void DMKEngine::updateSceneIndex(UI32 index)
	{
		instance.sceneIndex = index;
	}

	void DMKEngine::updateLevelIndex(UI32 index)
	{
		instance.levelIndex = index;
	}

	void DMKEngine::updateMovementSpeed(F32 speed)
	{
		instance.movementSpeed = speed;
	}

	void DMKEngine::updateMouseSensitivity(F32 sensitivity)
	{
		instance.mouseSensitivity = sensitivity;
	}

	void DMKEngine::updateAnimationSpeed(F32 speed)
	{
		instance.animationSpeed = speed;
	}

	void DMKEngine::setupCamera(Camera* camera)
	{
		instance.myCamera = camera;
	}

	void DMKEngine::terminateInstance()
	{
		ADGR::Renderer::terminate();
	}

	void DMKEngine::initializeRendererStageOne()
	{
		/* Terminate the StartupRenderer */
		ADGR::StartupRenderer::terminate();

		/* Initialize window */
		DMKWindowManagerInitInfo windowInitInfo;
		windowInitInfo.iconPaths = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg";
		instance.myWindowManager.initialize(windowInitInfo);

		/* Set basic initializing data to the rendering engine */
		ADGR::Renderer::setProgressPointer(&instance.progress);
		ADGR::Renderer::setWindowHandle(instance.myWindowManager.window);

		/* Get the renderable assets */
		ADGR::Renderer::setRenderableObjects(instance.myAssetManager.getRenderablesAsInternalFormats(instance.sceneIndex, instance.levelIndex));

		/* Initialize the Renderer: Stage One */
		ADGR::Renderer::initializeStageOne(instance.myInstanceDescriptor.renderingAPI, instance.myInstanceDescriptor.settings);
	}

	void DMKEngine::genarateRenderables()
	{
		/* Get the renderable assets in the AIB as POINTER<InternalFormat> */
		instance.internalFormats = instance.myAssetManager.getRenderablesAsInternalFormats(instance.sceneIndex, instance.levelIndex);
		
		/* Submit the assets to the renderer */
		ADGR::Renderer::setRenderableObjects(instance.internalFormats);
		ADGR::Renderer::initializeStageTwo();
	}

	void DMKEngine::addToRenderingQueue(DMKGameObject* object)
	{
		/* Create a new Asset Container, add it to the Asset Manager and submit to the rendering queue */
		AssetContainer _container;
		_container.address = (InternalFormat*)object;
		_container.byteSize = sizeof(*object);
		_container.type = object->type;

		/* Submit the asset to the Asset Manager. */
		instance.myAssetManager.addAsset(_container, instance.sceneIndex, instance.levelIndex);

		/* Submit the asset to the render queue */
		ADGR::Renderer::addToRenderQueue((InternalFormat*)object);
	}

	void DMKEngine::initializeRendererStageTwo()
	{
		ADGR::Renderer::initializeStageTwo();
	}

	void DMKEngine::initializeRendererStageThree()
	{
		ADGR::Renderer::initializeStageThree();
	}

	void DMKEngine::run()
	{
		/* Instanciate draw frame info structure */
		DMKRendererDrawFrameInfo info;
		info.aspectRatio = instance.aspectRatio;
		info.FOV = instance.FOV;
		info.frustumFar = instance.frustumFar;
		info.frustumNear = instance.frustumNear;

		/* Main execution loop of the Dynamik Engine									*/
		/* First it polls events and checks whether the window is still valid.			*/
		/* If the window is invalid (closed), it goes to the termination subroutine.	*/
		while (EventManager::pollEventsGLFW())
		{
			/* Get the events and store them locally */
			instance.eventComponents = EventManager::getEventComponents();

			/* Update the camera and store its tada locally */
			instance.cameraData = instance.myCamera->update(instance.eventComponents);

			/* Draw the frame using the camera data */
			info.cameraData = instance.cameraData;
			info.formats = instance.internalFormats;
			ADGR::Renderer::drawFrame(info);
		}

		/* Clean the rendering engine right after the window gets terminated */
		ADGR::Renderer::frameCleanup();
	}

	void DMKEngine::terminate()
	{
		ADGR::Renderer::terminate();
	}

	inline B1 DMKEngine::isRenderableAsset(AssetContainer asset)
	{
		return asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA;
	}
	
	inline void DMKEngine::cleanUniformBuffers()
	{
		/* Clean all the uniform buffer data to add new uniform data. */
		for (auto _format : instance.internalFormats)
			_format->uniformBufferData = {};
	}
}