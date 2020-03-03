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
#include "Platform/Windows.h"
#include "Networking.h"

#include "engineControlUnit.h"


/*
 TODO:
	Dynamik scripting support library
*/

namespace std {
	using Dynamik::Vertex;

	template<>
	struct hash<Dynamik::Vertex> {
		size_t operator()(Vertex const& vertexs) const {
			using Dynamik::Vertex;
			using glm::vec2;
			using glm::vec3;

			return ((hash<glm::vec3>()(vertexs.Position) ^
				(hash<glm::vec3>()(vertexs.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertexs.TexCoordinates) << 1);
		}
	};
}

// 35
namespace Dynamik {
	/* SAMPLE */
	class SampleThread : public Thread {
	public:
		UI32 count = 0;

		void init() override {
			std::cout << "Init Stage\n";
		}

		void loop() override {
			std::cout << "Running in Main Loop\n";

			if (count == 100000)
				terminate = true;
			count++;
		}

		void shutdown() override {
			std::cout << "Shuting Down\n";
		}
	};

	uint32_t progress = 0;
	static bool shouldClose = false;

	Application::Application(std::vector<Scene*>& _scenes) : scenes(_scenes) {
		if (_scenes[sceneCount]->myGameObjects.size() < 1) {
			DMK_CORE_ERROR("No Game Objects found!");
			return;
		}

		//FastString fstr = "Hello World";

		gameObjectInitialization();

		std::thread myThread(Application::showProgress);

		//myRenderingEngine.setProgress(&progress);
		//myRenderingEngine.setRendererFormats(internalFormatsBase);
		//myRenderingEngine.initRenderer();

		SampleThread thr;
		RendererThread rendererThread;
		rendererThread.addProgress(&progress);
		rendererThread.addInternalFormats(internalFormatsBase);
		myThreadManager.add(&thr);
		myThreadManager.add(&rendererThread);
		myThreadManager.runAll();

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
				auto events = myRenderingEngine.getEvents();

				myRenderingEngine.draw();
				myEngine.update();
				if (events->size())
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

	void Application::onEvent(std::deque<DMKEventContainer>* events) {
		for (int i = 0; i < events->size(); i++) {
			DMKEventContainer eventContainer = events->back();
			events->pop_back();

			if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS) {
				switch (eventContainer.code) {
				case DMK_KEY_F: {
					for (int x = 0; x < audioControllers.size(); x++) {
						audioControllers[x].isPaused = false;
						audioControllers[x].isLooped = false;
						myEngine.addAudioController(audioControllers[i]);
					}
					break;
				}
				case DMK_KEY_G:
					for (int x = 0; x < audioControllers.size(); x++) {
						audioControllers[x].isPaused = true;
						audioControllers[x].isLooped = true;
						myEngine.addAudioController(audioControllers[i]);
					}
					break;
				case DMK_KEY_O: {
					bool canFinish = false;
					GameObjectProperties props;
					props.name = "Debug Object";
					props.location = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/moon";
					props.transformProperties.location = { 0.0f, 5.0f, 0.0f };

					myObject.myProperties = props;

					std::thread _threadOne([](Application* application, bool* canFinish) {
						while (!(*canFinish)) {
							DMK_CORE_INFO("In Subroutine");
							auto events = application->myRenderingEngine.getEvents();

							application->myRenderingEngine.draw();
							application->myEngine.update();
							application->onEvent(events);

							for (auto layer : application->layerStack)
								layer->update();
						}
						}, this, &canFinish);

					getObjectPaths(&myObject);
					initAudioControllers(&myObject);
					std::vector<InternalFormat*> _localInternalFormatBases;
					initRendererFormats(&_localInternalFormatBases);
					myRenderingEngine.loadDataToUpdate(_localInternalFormatBases);
					canFinish = true;
					_threadOne.join();
					myRenderingEngine.updateRendererFormats();
					internalFormatsBase = _localInternalFormatBases;

					//std::thread _threadOne([](Application* application, GameObject* myObject, bool* canFinish) {
					//	application->getObjectPaths(myObject);
					//	application->initAudioControllers(myObject);
					//	std::vector<InternalFormat*> _localInternalFormatBases;
					//	application->initRendererFormats(&_localInternalFormatBases);
					//	application->myRenderingEngine.loadDataToUpdate(_localInternalFormatBases);
					//	application->myRenderingEngine.updateRendererFormats();
					//	*canFinish = true;
					//	application->internalFormatsBase = _localInternalFormatBases;
					//	}, this, &myObject, &canFinish);
					//_threadOne.detach();
					//
					//while (!canFinish) {
					//	auto events = myRenderingEngine.getEvents();
					//
					//	myRenderingEngine.draw();
					//	myEngine.update();
					//	onEvent(events);
					//
					//	DMK_CORE_INFO("In Subroutine");
					//
					//	for (auto layer : layerStack)
					//		layer->update();
					//}

					//if (!_threadOne.joinable())
					//	_threadOne.join();

					break;
				}
				default:
					break;
				}
			}
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
#endif
	}

	void Application::gameObjectInitialization() {
		getObjectPaths();
		//initAudioControllers();
		//loadObjectData();
		initRendererFormats();
	}

	void Application::getObjectPaths() {
		gameObjects = scenes[sceneCount]->myGameObjects;

		for (int itr = 0; itr < gameObjects.size(); itr++) {
			if (
				(gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_MESH)
				|| (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
				|| (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
				|| (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PLAYER)
				|| (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_NPC)
				|| (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_CUSTOM)
				) {
				GameObject* gameObject = gameObjects[itr];

				fileManager.open(gameObject->myProperties.location + (
					(gameObject->myProperties.location[gameObject->myProperties.location.size() - 1] == '/')
					? "modelData.dai" : "/modelData.dai"));

				if (!fileManager.isOpen())
					DMK_CORE_FATAL("modelData.dai file not found --> " + gameObject->myProperties.location);

				std::string basePath = "";
				if (gameObject->myProperties.location[gameObject->myProperties.location.size() - 1] == '/')
					basePath = gameObject->myProperties.location;
				else
					basePath = gameObject->myProperties.location + (CHR)'/';

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

			internalFormats.push_back(internalFormat(gameObjects[itr]));
		}
	}

	void Application::getObjectPaths(GameObject* object) {
		if (
			(object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_MESH)
			|| (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
			|| (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
			|| (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PLAYER)
			|| (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_NPC)
			|| (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_CUSTOM)
			) {
			GameObject* gameObject = object;

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

		internalFormats.push_back(internalFormat(object));
	}

	void Application::loadObjectData() {
		{
			std::vector<std::future<void>> threads = {};
			for (int i = 0; i < internalFormats.size(); i++) {
				if (renderableObjectCheck(internalFormats[i])) {
					internalFormats[i].myVertexBufferObjects.resize(1);
					internalFormats[i].myIndexBufferObjects.resize(1);

					//DMKModelLoadInfo loadInfo = {};
					//loadInfo.path = internalFormats[i].myGameObject->myProperties.objectPath[0];
					//loadInfo.vertices = &internalFormats[i].myVertexBufferObjects[0];
					//loadInfo.indices = &internalFormats[i].myIndexBufferObjects[0];
					//loadInfo.vertexOffset = internalFormats[i].myGameObject->myProperties.transformProperties.location;

					//threads.push_back(std::async(std::launch::async, loadModel, loadInfo));
					//threads.push_back(std::async(std::launch::async, utils::loadObjFileData, &internalFormats[i]));
					//.push_back(std::async(std::launch::async, loadModel, loadInfo));
					//utils::loadObjFileData(&internalFormats[i]);
					//
					//internalFormats[i].myVertexBufferObjects.resize(1);
					//internalFormats[i].myIndexBufferObjects.resize(1);
					//
					//DMKModelLoadInfo loadInfo = {};
					//loadInfo.path = internalFormats[i].myGameObject->myProperties.objectPath[0];
					//loadInfo.vertices = &internalFormats[i].myVertexBufferObjects[0];
					//loadInfo.indices = &internalFormats[i].myIndexBufferObjects[0];
					//loadInfo.vertexOffset = internalFormats[i].myGameObject->myProperties.transformProperties.location;
					//loadModel(loadInfo);
				}
			}
		}

		for (int i = 0; i < internalFormats.size(); i++) {
			if (renderableObjectCheck(internalFormats[i])) {
				internalFormats[i].myVertexCounts.resize(1);
				internalFormats[i].myVertexCounts[0] = internalFormats[i].myVertexBufferObjects[0].size();
				internalFormats[i].myIndexCounts.resize(1);
				internalFormats[i].myIndexCounts[0] = internalFormats[i].myIndexBufferObjects[0].size();
			}
		}
	}

	void Application::initRendererFormats() {
		internalFormatsBase.clear();
		for (int i = 0; i < internalFormats.size(); i++)
			if (renderableObjectCheck(internalFormats[i])) {
				internalFormats[i].isInitializedPrimary = true;
				internalFormatsBase.push_back(&internalFormats[i]);
			}
	}

	void Application::initRendererFormats(std::vector<InternalFormat*>* formats) {
		for (int i = 0; i < internalFormats.size(); i++)
			if (renderableObjectCheck(internalFormats[i])) {
				internalFormats[i].isInitializedPrimary = true;
				formats->push_back(&internalFormats[i]);
			}
	}

	void Application::initAudioControllers() {
		for (int itr = 0; itr < gameObjects.size(); itr++)
			if (gameObjects[itr]->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_AUDIO)
				audioControllers.push_back(Audio::BasicAudioController((gameObjects[itr]->myProperties.objectPath[0])));
	}

	void Application::initAudioControllers(GameObject* object) {
		if (object->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_AUDIO)
			audioControllers.push_back(Audio::BasicAudioController((object->myProperties.objectPath[0])));
	}

	bool Application::renderableObjectCheck(internalFormat format) {
		return (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_MESH)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PLAYER)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_NPC)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SKYBOX)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_SPRITES)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_FONT)
			|| (format.myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_PARTICLE);
	}
}