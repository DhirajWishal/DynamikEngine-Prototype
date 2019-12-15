#pragma once

#ifndef _DYNAMIK_APPLICATION_H
#define _DYNAMIK_APPLICATION_H

#include "core.h"
#include "src/Renderer.h"
#include "src/rendererFormat.h"
#include "src/Layers/layer.h"
#include "src/Layers/layerStack.h"

#include "inputHandler.h"

#include "Audio.h"

#include "data store/internalFormat.h"
#include "object mechanics/loadGameObjects.h"
#include "Level.h"

namespace Dynamik {
	/* Local internal format instance */
	class internalFormat : public InternalFormat {
	public:
		internalFormat(GameObject* object) : InternalFormat(object) {}
		~internalFormat() {}
	};

	class DMK_API Application {
	public:
		Application(std::vector<Scene*>& gameObjects);
		virtual ~Application();

		void run();

		void pushLayer(ADGR::Layer* layer);
		void pushOverlay(ADGR::Layer* layer);

		void onEvent(std::deque<DMKEventContainer> events);

		static void showProgress();

	private:
		void getObjectPaths();
		void loadObjectData();
		void initRendererFormats();

		bool initSuccessful = false;

		ADGR::Renderer myRenderingEngine;

		std::vector<internalFormat> internalFormats = {};
		std::vector<InternalFormat*> internalFormatsBase = {};

		std::vector<const void*> references = {};

		std::vector<Level*> levels = {};
		std::vector<Scene*> scenes = {};
		std::vector<GameObject*> gameObjects = {};
		std::vector<ADGR::RendererFormat*> rendererFormats = {};

		ADGR::layerStack layerStack;

		Audio::AudioEngine myEngine;

		uint32_t sceneCount = 0;
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
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,													\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT,													\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,															\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI,																	\
			Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT,															\
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
