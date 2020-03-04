#pragma once

#ifndef _DYNAMIK_APPLICATION_H
#define _DYNAMIK_APPLICATION_H

#include "core.h"

#include "inputHandler.h"

#include "Audio.h"

#include "data store/internalFormat.h"
#include "object mechanics/loadGameObjects.h"
#include "Level.h"
#include "Events.h"

#include "Managers.h"

namespace Dynamik {
	class DebugObject : public GameObject {
	public:
		DebugObject() {}
		DebugObject(GameObjectProperties props) : GameObject(props) {}
		~DebugObject() {}

		DMKUpdateInfo draw(std::deque<DMKEventContainer>& eventContainers) override {
			for (int i = 0; i < eventContainers.size(); i++) {
				DMKEventContainer eventContainer = eventContainers.back();
				eventContainers.pop_back();

				if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_PRESS)
					switch (eventContainer.code) {
					case DMK_KEY_W:
						myUpdateProperties.frontBack += myUpdateProperties.movementBiasFrontBack;
						break;
					case DMK_KEY_A:
						myUpdateProperties.leftRight -= myUpdateProperties.movementBiasLeftRight;
						break;
					case DMK_KEY_S:
						myUpdateProperties.frontBack -= myUpdateProperties.movementBiasFrontBack;
						break;
					case DMK_KEY_D:
						myUpdateProperties.leftRight += myUpdateProperties.movementBiasLeftRight;
						break;
					case DMK_KEY_UP:
						myUpdateProperties.upDown -= myUpdateProperties.movementBiasUpDown;
						break;
					case DMK_KEY_DOWN:
						myUpdateProperties.upDown += myUpdateProperties.movementBiasUpDown;
						break;
					case DMK_KEY_LEFT:
						myUpdateProperties.rotationZ -= myUpdateProperties.rotationBias;
						break;
					case DMK_KEY_RIGHT:
						myUpdateProperties.rotationZ += myUpdateProperties.rotationBias;
						break;
					}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT)
					switch (eventContainer.code) {
					case DMK_KEY_W:
						myUpdateProperties.frontBack += myUpdateProperties.movementBiasFrontBack;
						break;
					case DMK_KEY_A:
						myUpdateProperties.leftRight -= myUpdateProperties.movementBiasLeftRight;
						break;
					case DMK_KEY_S:
						myUpdateProperties.frontBack -= myUpdateProperties.movementBiasFrontBack;
						break;
					case DMK_KEY_D:
						myUpdateProperties.leftRight += myUpdateProperties.movementBiasLeftRight;
						break;
					case DMK_KEY_UP:
						myUpdateProperties.upDown -= myUpdateProperties.movementBiasUpDown;
						break;
					case DMK_KEY_DOWN:
						myUpdateProperties.upDown += myUpdateProperties.movementBiasUpDown;
						break;
					case DMK_KEY_LEFT:
						myUpdateProperties.rotationZ -= myUpdateProperties.rotationBias;
						break;
					case DMK_KEY_RIGHT:
						myUpdateProperties.rotationZ += myUpdateProperties.rotationBias;
						break;
					}
				else if (eventContainer.eventType == DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED)
					myUpdateProperties.rotationX = eventContainer.xAxis;
				myUpdateProperties.rotationY = eventContainer.yAxis;
			}

			return myUpdateProperties;
		}
	};

	/* Local internal format instance */
	class internalFormat : public InternalFormat {
	public:
		internalFormat(GameObject* object) : InternalFormat(object) {}
		~internalFormat() {}
	};

	enum class DMKRendererAPI {
		DMK_RENDERER_API_VULKAN,
		DMK_RENDERER_API_DIRECT_X_12,
		DMK_RENDERER_API_OPENGL,

		DMK_RENDERER_API_UNDEFNED
	};

	struct DMKEngineSettings {
		std::string myInstanceName = "Dynamik Engine";
		std::string myInstanceID = "01";

		DMKRendererAPI myRendererAPI = DMKRendererAPI::DMK_RENDERER_API_VULKAN;
	};

	class DMK_API Application {
	public:
		Application(std::vector<Scene*>& gameObjects);
		virtual ~Application();

		void run();

		void pushLayer(ADGR::Layer* layer);
		void pushOverlay(ADGR::Layer* layer);

		void onEvent(std::deque<DMKEventContainer>* events);

		static void showProgress();

		void gameObjectInitialization();
		void getObjectPaths();
		void getObjectPaths(GameObject* object);
		void loadObjectData();
		void initRendererFormats();
		void initRendererFormats(std::vector<InternalFormat*>* formats);
		void initAudioControllers();
		void initAudioControllers(GameObject* object);

		bool renderableObjectCheck(internalFormat format);
	
	private:
		bool initSuccessful = false;
		bool canDeleteController = false;

		ADGR::Renderer myRenderingEngine;
		ThreadManager myThreadManager;

		std::vector<internalFormat> internalFormats = {};
		std::vector<InternalFormat*> internalFormatsBase = {};

		std::vector<const void*> references = {};

		std::vector<Level*> levels = {};
		std::vector<Scene*> scenes = {};
		std::vector<GameObject*> gameObjects = {};
		DebugObject myObject;
		std::vector<ADGR::RendererFormat*> rendererFormats = {};
		std::vector<Audio::BasicAudioController> audioControllers = {};

		ADGR::layerStack layerStack;

		Audio::AudioEngine myEngine;

		uint32_t sceneCount = 0;

		/* MANAGERS */
		utils::daiManager fileManager;

		/* THREADS */

	};

	// Defined by the Client
	//Application* createApplication();
	Application* createApplication(std::vector<Dynamik::Scene*>& gameObjects);
}

#define DYNAMIK_ENGINE_MAIN()																				\
int main(int argc, char** argv) {																			\
	try {																									\
		auto paths = getTexturePath(readFile("E:/Projects/Dynamik Engine/Dynamik/Application/paths.txt"));	\
																											\
		std::vector<Dynamik::GameObject*> gameObjects = {};													\
																											\
		std::vector<std::vector<float>> locations = {														\
			{0.0f,	0.0f,	0.0f},																			\
			{0.0f,	5.0f,	0.0f},																			\
			{0.0f,	-5.0f,	0.0f},																			\
			{0.0f,	10.0f,	0.0f},																			\
			{0.0f,	-10.0f,	0.0f},																			\
																											\
			{5.0f,	0.0f,	0.0f},																			\
			{5.0f,	5.0f,	0.0f},																			\
			{5.0f,	-5.0f,	0.0f},																			\
			{5.0f,	10.0f,	0.0f},																			\
			{5.0f,	-10.0f,	0.0f},																			\
																											\
			{-5.0f,	0.0f,	0.0f},																			\
			{-5.0f,	5.0f,	0.0f},																			\
			{-5.0f,	-5.0f,	0.0f},																			\
			{-5.0f,	10.0f,	0.0f},																			\
			{-5.0f,	-10.0f,	0.0f},																			\
																											\
			{10.0f,	0.0f,	0.0f},																			\
			{10.0f,	5.0f,	0.0f},																			\
			{10.0f,	-5.0f,	0.0f},																			\
			{10.0f,	10.0f,	0.0f},																			\
			{10.0f,	-10.0f,	0.0f},																			\
																											\
			{-10.0f,	0.0f,	0.0f},																		\
			{-10.0f,	5.0f,	0.0f},																		\
			{-10.0f,	-5.0f,	0.0f},																		\
			{-10.0f,	10.0f,	0.0f},																		\
			{-10.0f,	-10.0f,	0.0f},																		\
		};																									\
																											\
		std::vector<Dynamik::DMKObjectType> objTypes = {													\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,										\			\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,										\			\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,											\				\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI,														\			\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,											\				\
		};																									\
																											\
		for (int i = 0; i < paths.size(); i++) {															\
			Dynamik::GameObjectProperties props;															\
			props.name = "Charlet";																			\
			props.ID = std::to_string(i);																	\
			props.location = paths[i];																		\
			props.transformProperties.location = locations[i];												\
																											\
			gameObjects.push_back(charlet(props));															\
		}																									\
																											\
		auto application = Dynamik::createApplication(gameObjects);											\
		application->run();																					\
																											\
		delete application;																					\
	}																										\
	catch (std::exception & e) {																			\
		std::cout << e.what();																				\
																											\
		return DMK_FAIL;																					\
	}																										\
																																				\
	return DMK_SUCCESS;																						\
}

#endif // !_DYNAMIK_APPLICATION_H
