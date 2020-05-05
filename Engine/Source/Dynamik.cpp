/*
 Dynamik Engine API source file.

 Author:	Dhiraj Wishal
 Date:		12/04/2020
 IDE:		MS Visual Studio Community 2019

 This file contains all the necessary definitions for the Dynamik Engine API.
*/

#include "dmkafx.h"
#include "Dynamik.h"

#include "Renderer/StartupRenderer/StartupRenderer.h"

#include "Events/EventManager.h"

#include "core/benchmark/profiler.h"

namespace Dynamik {
	/* Dynamik instance */
	DMKEngine DMKEngine::instance;

	/* For internal use */
	static B1 finishStartUpRenderer = false;

	void DMKEngine::initializeInstance(DMKInstanceDescriptor descriptor)
	{
		DMK_BEGIN_PROFILING();
		DMK_BEGIN_PROFILE_TIMER();

		/* Initialize the startup renderer to render basic data while the main renderer is activated.		*/
		/* This way the user is not presented with a blank screen while all the scene data are loaded.		*/
		/* The startup renderer is run on a separate thread to make sure that it wont affect the Engine.	*/
		instance.myStartypRendererThread.swap(std::thread(
			[](CCPTR iconPath)
			{
				Renderer::StartupRenderer::loadTexure("E:/Projects/Dynamik Engine/Versions/Dynamik/Dependencies/Assets/icons/Samples/Dynamik.png");
				Renderer::StartupRenderer::initialize();
				Renderer::StartupRenderer::initializeShaders();
				Renderer::StartupRenderer::initializeVertexBuffers();
				Renderer::StartupRenderer::initializeTextureImage();

				while (!finishStartUpRenderer)
				{
					DMK_BEGIN_PROFILE_TIMER();

					Renderer::StartupRenderer::draw();
				}

				Renderer::StartupRenderer::terminate();

			}, descriptor.iconPath
			));
	}

	UI32 DMKEngine::addLevel(DMKLevelDescriptor level)
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Since each level consists of one or more scenes, we pack the given scenes to a wrapper.	*/
		/* After wrapping them in scene wrappers, we add the assets to it.							*/
		ARRAY<ARRAY<AssetContainer>> scenes;
		for (auto scene : level.scenes)
		{
			DMK_BEGIN_PROFILE_TIMER();

			ARRAY<AssetContainer> assets;
			for (auto object : scene.assets)
				assets.pushBack(AssetManager::createAssetContainer(object));

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

	void DMKEngine::loadCurrentScene()
	{
		/* Get the renderable assets in the AIB as POINTER<InternalFormat> */
		instance.myAssetManager.loadScene(instance.sceneIndex, instance.levelIndex);
		instance.internalFormats = instance.myAssetManager.getRenderablesAsInternalFormats(instance.sceneIndex, instance.levelIndex);
	}

	UI32 DMKEngine::addAsset(DMKGameObject* object)
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Add the asset to the Asset Manager and return its index. */
		return instance.myAssetManager.addAsset(object, instance.sceneIndex, instance.levelIndex);
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

	void DMKEngine::setupCamera(DMKCamera* camera)
	{
		instance.myCamera = camera;
	}

	void DMKEngine::terminateInstance()
	{
		Renderer::DMKRenderer::terminate();
	}

	void DMKEngine::initializeRendererStageOne()
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Initialize window */
		auto windowHandle = WindowManager::createWindow(DMKWindowInitInfo());

		/* Set basic initializing data to the rendering engine */
		Renderer::DMKRenderer::setProgressPointer(&instance.progress);
		Renderer::DMKRenderer::setWindowHandle(windowHandle);
		Renderer::DMKRenderer::setWindowExtent(windowHandle->windowWidth, windowHandle->windowHeight);

		/* Initialize the Renderer: Stage One */
		Renderer::DMKRenderer::initializeStageOne(instance.myInstanceDescriptor.renderingAPI, instance.myInstanceDescriptor.settings);
	}

	void DMKEngine::genarateRenderables()
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Submit the assets to the renderer */
		Renderer::DMKRenderer::setRenderableObjects(instance.internalFormats);
	}

	void DMKEngine::submitLoadedAssets()
	{
		Renderer::DMKRenderer::submitLoadedAssets();
	}

	void DMKEngine::addToRenderingQueue(DMKGameObject* object)
	{
		/* Submit the asset to the Asset Manager. */
		instance.myAssetManager.addAsset(object, instance.sceneIndex, instance.levelIndex);

		/* Submit the asset to the render queue */
		Renderer::DMKRenderer::addToRenderQueue((InternalFormat*)object);
	}

	void DMKEngine::initializeRendererStageTwo()
	{
		DMK_BEGIN_PROFILE_TIMER();

		Renderer::DMKRenderer::initializeStageTwo();
	}

	void DMKEngine::initializeRendererStageThree()
	{
		DMK_BEGIN_PROFILE_TIMER();

		Renderer::DMKRenderer::initializeStageThree();
	}

	void DMKEngine::run()
	{
		DMK_BEGIN_PROFILE_TIMER();

		/* Terminate the StartupRenderer */
		finishStartUpRenderer = true;
		instance.myStartypRendererThread.join();

		/* Instanciate draw frame info structure */
		DMKRendererDrawFrameInfo info;

		/* Main execution loop of the Dynamik Engine									*/
		/* First it polls events and checks whether the window is still valid.			*/
		/* If the window is invalid (closed), it goes to the termination subroutine.	*/
		while (EventManager::pollEventsGLFW())
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Clean used variables for a new draw call */
			onUpdateCleanup();

			/* Get the events and store them locally */
			instance.eventComponents = EventManager::getEventComponents();

			/* Update the camera and store its tada locally */
			instance.cameraData = instance.myCamera->update(instance.eventComponents);

			/* Draw the frame using the camera data */
			info.cameraData = instance.cameraData;
			Renderer::DMKRenderer::drawFrame(info);
		}

		/* Clean the rendering engine right after the window gets terminated */
		Renderer::DMKRenderer::frameCleanup();
	}

	void DMKEngine::terminate()
	{
		Renderer::DMKRenderer::terminate();

		DMK_END_PROFILING();
	}

	inline B1 DMKEngine::isRenderableAsset(AssetContainer asset)
	{
		return asset.type <= DMKObjectType::DMK_OBJECT_TYPE_CAMERA;
	}

	inline void DMKEngine::cleanUniformBuffers()
	{
		/* Clean all the uniform buffer data to add new uniform data. */
	}

	inline void DMKEngine::onUpdateCleanup()
	{
		cleanUniformBuffers();
		instance.eventComponents = {};
	}

}