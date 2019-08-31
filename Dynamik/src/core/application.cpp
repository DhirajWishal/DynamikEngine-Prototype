#include "dmkafx.h"
#include "application.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"

namespace Dynamik {

	Application::Application() {
		myLoader.run();

		printf("%s %s\n", myLoader.getTexturePaths()[0].c_str(), myLoader.getModelPaths()[0].c_str());
		myRenderingEngine.setAssetPaths(myLoader.getTexturePaths(), myLoader.getModelPaths());

		myRenderingEngine.setMipLevel(1.0f);

		myRenderingEngine.initRenderer();
	}

	Application::~Application() {
		myRenderingEngine.end();
	}

	DMK_API void Application::run() {
		while (!myRenderingEngine.getWindowCloseEvent()) {
			myRenderingEngine.draw();
			//myEvent = &myRenderingEngine.pollEvents();
			myEngine.update();
			//my shitty code

			auto [keyEvent, mouseEvent] = myRenderingEngine.pollEvents();

			//if (keyEvent != nullptr)
			//	onEvent(*keyEvent);

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

	void Application::onEvent(keyEventData ked) {
		KeyPressedEvent keyPressed = ked.keyPressedEvent;
		KeyReleasedEvent keyReleased = ked.keyReleasedEvent;
		KeyPressedEvent keyRepeat = ked.keyRepeatEvent;

		static Audio::BasicAudioController audController("media/shooting.mp3");

		switch (keyPressed.getkeyCode()) {
		case DMK_KEY_D: {
			audController.isPaused = false;
			myEngine.addAudioController(audController);
			break;
		}
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
}