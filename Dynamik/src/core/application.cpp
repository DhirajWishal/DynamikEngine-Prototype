#include "dmkafx.h"
#include "application.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

#include "utils/DMK_Descriptors.h"

#include "debugger.h"

#include "core/Windows.h"
#include "data store/containers.h"
#include "modelManager.h"

/*
	TODO:
		Dynamik scripting support library
*/

// 35
namespace Dynamik {
	uint32_t progress = 0;
	static bool shouldClose = false;

	Application::Application(std::vector<Scene*>& _scenes) : scenes(_scenes) {
		if (_scenes[sceneCount]->myGameObjects.size() < 1) {
			DMK_CORE_ERROR("No Game Objects found!");
			return;
		}

		getObjectPaths();
		for (int i = 0; i < gameObjects.size(); i++)
			internalFormats.push_back(internalFormat(gameObjects[i]));
		loadObjectData();

		std::thread myThread(Application::showProgress);

		initRendererFormats();
		for (int i = 0; i < internalFormats.size(); i++)
			internalFormatsBase.push_back(&internalFormats[i]);
		myRenderingEngine.setRendererFormats(internalFormatsBase);
		myRenderingEngine.setProgress(&progress);
		myRenderingEngine.initRenderer();
		shouldClose = true;

		myThread.join();

		initSuccessful = true;
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		if (initSuccessful) {
			while (!myRenderingEngine.getWindowCloseEvent()) {
				myRenderingEngine.draw();
				myEngine.update();
				auto events = myRenderingEngine.getEvents();
				onEvent(events);

				for (auto layer : layerStack)
					layer->update();
			}

			myRenderingEngine.idleCall();
		}
	}

	void Application::pushLayer(ADGR::Layer* layer) {
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(ADGR::Layer* layer) {
		layerStack.pushOverLay(layer);
	}

	void Application::onEvent(std::deque<DMKEventContainer> events) {
		for (int i = 0; i < events.size(); i++) {
			DMKEventContainer eventContainer = events.back();
			events.pop_back();

			if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS) {
				switch (eventContainer.code) {
				case DMK_KEY_F: {
					Audio::BasicAudioController audController("E:/Projects/Dynamik Engine/Dynamik/components/Audio/media/explosion.wav");
					audController.isPaused = false;
					audController.isLooped = false;

					//Audio code
					myEngine.addAudioController(audController);
					//delete audController;
					break;
				}
				}
			}
			//if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT) {
			//	switch (eventContainer.code) {
			//
			//	}
			//}
			//if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED) {
			//
			//}
		}
		//for (auto it = layerStack.end(); it != layerStack.begin();) {
		//	(*--it)->onEvent(*event.event);
		//	if (event.event->handled)
		//		break;
	}

	void Application::showProgress() {
		static uint32_t old = 0;
		static uint8_t count = 0;
		static float percentage = 0.0f;

		static std::vector<std::string> symbols = {
			"|",
			"/",
			"-",
			"\\"
		};

		while (!shouldClose) {
			percentage = ((float)progress / 19.0) * 100.0;
#ifdef DMK_DEBUG
			printf("\rProgress: %f%%\t%s", percentage, symbols[count].c_str());
#endif
			count++;

			if (count > 3)	count = 0;

			old = progress;
			Sleep(100);
		}
#ifdef DMK_DEBUG
		printf("\rProgress: %f%%\t%s\t\n", 100.0f, symbols[count].c_str());

		printf("ADGR Initiated! Let the rendering begin!\n");
#endif
	}

	void Application::getObjectPaths() {
		gameObjects = scenes[sceneCount]->myGameObjects;

		for (int itr = 0; itr < gameObjects.size(); itr++) {
			GameObject* gameObject = gameObjects[itr];

			utils::daiManager fileManager;
			fileManager.open(gameObject->myProperties.location + (
				(gameObject->myProperties.location[gameObject->myProperties.location.size() - 1] == '/')
				? "modelData.dai" : "/modelData.dai"));

			if (!fileManager.isOpen())
				DMK_CORE_FATAL("modelData.dai file not found --> " + gameObject->myProperties.location);

			std::string basePath = "";
			if (gameObject->myProperties.location[gameObject->myProperties.location.size() - 1] == '/')
				basePath = gameObject->myProperties.location;
			else
				basePath = gameObject->myProperties.location + '/';

			// get model paths
			for (auto modelPath : fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_MODEL))
				gameObject->myProperties.objectPath.push_back(basePath + modelPath);

			// get texture paths
			for (auto texturePath : fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TEXTURE))
				gameObject->myProperties.texturePaths.push_back(basePath + texturePath);

			// get shader paths
			gameObject->myProperties.renderableObjectProperties.vertexShaderPath = (
				(fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX).size() > 0) ?
				basePath + fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_VERTEX)[0] : "NONE"
				);	// vertex shader
			gameObject->myProperties.renderableObjectProperties.tessellationShaderPath = (
				(fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION).size() > 0) ?
				basePath + fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_TESSELLATION)[0] : "NONE"
				);	// tessellation shader
			gameObject->myProperties.renderableObjectProperties.geometryShaderPath = (
				(fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY).size() > 0) ?
				basePath + fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_GEOMETRY)[0] : "NONE"
				);	// geometry shader
			gameObject->myProperties.renderableObjectProperties.fragmentShaderPath = (
				(fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT).size() > 0) ?
				basePath + fileManager.getData(utils::DMK_DAI_FILE_DATA_TYPE_FRAGMENT)[0] : "NONE"
				);	// fragment shader
		}
	}

	void Application::loadObjectData() {
		std::vector<std::future<void>> threads = {};
		for (int i = 0; i < internalFormats.size(); i++) {
			internalFormats[i].myVertexBufferObjects.resize(1);
			internalFormats[i].myIndexBufferObjects.resize(1);
			//internalFormats[i]->initVertedAndIndexBufferObjects(1);

			DMKModelLoadInfo loadInfo = {};
			loadInfo.path = internalFormats[i].myGameObject->myProperties.objectPath[0];
			loadInfo.vertices = &internalFormats[i].myVertexBufferObjects[0];
			loadInfo.indices = &internalFormats[i].myIndexBufferObjects[0];
			loadInfo.vertexOffset = internalFormats[i].myGameObject->myProperties.transformProperties.location;

			threads.push_back(std::async(std::launch::async, loadModel, loadInfo));
		}
	}

	void Application::initRendererFormats() {
		for (int i = 0; i < internalFormats.size(); i++) {
			if ((internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PLAYER)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_NPC)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SPRITES)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_FONT)
				|| (internalFormats[i].myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PARTICLE)
				)
				rendererFormats.push_back(&ADGR::RendererFormat((InternalFormat*)&internalFormats[i]));
		}
	}
}