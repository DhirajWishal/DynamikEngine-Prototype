#include "dmkafx.h"
#include "application.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

#include "utils/DMK_Descriptors.h"

#include "debugger.h"

#include "core/Windows.h"

/*
	TODO:
		Dynamik scripting support library
*/

// 35
namespace Dynamik {
	uint32_t progress = 0;
	static bool shouldClose = false;

	Application::Application() {
		//gameObjectsWrapper = myLoader.run();

		if (gameObjects.size() < 1) {
			DMK_CORE_ERROR("No Game Objects found!");
			return;
		}

		getObjectPaths();

		std::thread myThread(Application::showProgress);

		std::vector<std::vector<std::string>>& texture = myLoader.getTexturePaths();
		std::vector<std::string>& model = myLoader.getModelPaths();

		myRenderingEngine.getGameObjects(gameObjects);
		myRenderingEngine.initRenderer({ "", 1.0f,  model, texture,  &progress });
		shouldClose = true;

		myThread.join();

		initSuccessful = true;
	}

	Application::Application(std::vector<GameObject>& _gameObjects) : gameObjects(_gameObjects){
		if (gameObjects.size() < 1) {
			DMK_CORE_ERROR("No Game Objects found!");
			return;
		}


		getObjectPaths();

		std::thread myThread(Application::showProgress);

		std::vector<std::vector<std::string>>& texture = myLoader.getTexturePaths();
		std::vector<std::string>& model = myLoader.getModelPaths();

		myRenderingEngine.getGameObjects(gameObjects);
		myRenderingEngine.initRenderer({ "", 1.0f,  model, texture,  &progress });
		shouldClose = true;

		myThread.join();

		initSuccessful = true;
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		while (!myRenderingEngine.getWindowCloseEvent()) {
			myRenderingEngine.draw();
			myEngine.update();
			//my shitty code

			auto [keyEvent, mouseEvent] = myRenderingEngine.pollEvents();

			onEvent(keyEvent);

			for (auto layer : layerStack)
				layer->update();
		}

		myRenderingEngine.idleCall();
	}

	void Application::pushLayer(ADGR::Layer* layer) {
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(ADGR::Layer* layer) {
		layerStack.pushOverLay(layer);
	}

	void Application::onEvent(int ked) {
		//KeyPressedEvent keyPressed = ked.keyPressedEvent;
		//KeyReleasedEvent keyReleased = ked.keyReleasedEvent;
		//KeyPressedEvent keyRepeat = ked.keyRepeatEvent;

		switch (ked) {
		case DMK_KEY_F: {
			Audio::BasicAudioController* audController = new Audio::BasicAudioController("E:/Projects/Dynamik Engine/Dynamik/components/Audio/media/explosion.wav");
			audController->isPaused = false;
			audController->isLooped = false;

			//Audio code
			myEngine.addAudioController(*audController);
			break;
		}
		case DMK_KEY_SPACE:
			break;
		default:
			break;
		}

		//EventDispatcher dispatcher(*event.event);
		//
		//DMK_CORE_INFO("Im on Event!");
		//
		//static Audio::BasicAudioController audController("media/shooting.mp3");
		//
		//KeyPressedEvent* kpe = nullptr;
		//KeyReleasedEvent* kre;
		//
		//switch (event.type) {
		//case KEY_PRESSED_EVENT:
		//	*kpe = dynamic_cast<KeyPressedEvent&>(*event.event);
		//	printf("%s\n", kpe->toString());
		//	break;
		//}
		//
		//printf("%d\n", event.event->getEventType());
		//
		//switch (event.event->getEventType()) {
		//case EVENT_TYPE::keyPressed:
		//	audController.isPaused = false;
		//	myEngine.addAudioController(audController);
		//	break;
		//case EVENT_TYPE::keyReleased:
		//	audController.isPaused = true;
		//	break;
		//}
		//
		//
		//for (auto it = layerStack.end(); it != layerStack.begin();) {
		//	(*--it)->onEvent(*event.event);
		//	if (event.event->handled)
		//		break;
		//
		//}
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
			percentage = ((float)progress / 34.0) * 100.0;
			printf("\rProgress: %f%%\t%s", percentage, symbols[count].c_str());
			count++;

			if (count > 3)	count = 0;

			old = progress;
			Sleep(100);
		}
		printf("\rProgress: %f%%\t%s\t\n", 100.0f, symbols[count].c_str());

		printf("ADGR Initiated! Let the rendering begin!\n");
	}

	void Application::getObjectPaths() {
		for (int itr = 0; itr < gameObjects.size(); itr++) {
			GameObject* gameObject = &gameObjects[itr];

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
}