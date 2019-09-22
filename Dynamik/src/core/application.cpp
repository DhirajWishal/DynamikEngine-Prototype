#include "dmkafx.h"
#include "application.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"
#include "keyCodes.h"
// 34
namespace Dynamik {
	void worker() {
	}

	uint32_t progress = 0;
	static bool shouldClose = false;

	Application::Application() {
		myLoader.run();

		myRenderingEngine.setAssetPaths(myLoader.getTexturePaths(), myLoader.getModelPaths());

		myRenderingEngine.setMipLevel(1.0f);

		myRenderingEngine.setProgress(&progress);
		std::thread myThread(Application::showProgress);

		myRenderingEngine.initRenderer();
		shouldClose = true;

		myThread.join();
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

		static Audio::BasicAudioController audController
			("E:/Projects/Dynamik Engine/Dynamik/components/Audio/media/explosion.wav");

		switch (ked) {
		case DMK_KEY_F: {
			audController.isPaused = false;
			myEngine.addAudioController(audController);
			//Sleep(1000);
			break;
		}
		case DMK_KEY_SPACE:
			audController.isPaused = true;
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

		static std::string progressBar = "";
		static char prog = '-';

		//while (percentage <= 100.0f) {
		while (!shouldClose) {
			switch (((int)percentage / 10) * 10) {
			case 0:
				progressBar = "|";
				break;
			case 10:
				progressBar = "|--                  |";
				break;
			case 20:
				progressBar = "|----                |";
				break;
			case 30:
				progressBar = "|------              |";
				break;
			case 40:
				progressBar = "|--------            |";
				break;
			case 50:
				progressBar = "|----------          |";
				break;
			case 60:
				progressBar = "|------------        |";
				break;
			case 70:
				progressBar = "|--------------      |";
				break;
			case 80:
				progressBar = "|----------------    |";
				break;
			case 90:
				progressBar = "|------------------  |";
				break;
			case 100:
				progressBar = "|--------------------|";
				break;
			}

			percentage = ((float)progress / 34.0) * 100.0;
			printf("\rProgress: %f%%\t%s\t%s", percentage, symbols[count].c_str(),
				progressBar.c_str());
			count++;

			//if (old != progress && progress != 0)
			//	progressBar += prog;
			if (count > 3)count = 0;

			old = progress;
			Sleep(100);
		}
		printf("\rProgress: %f%%\t%s\t%s\n", 100.0f, symbols[count].c_str(), "|--------------------|");

		printf("ADGR Initiated! Let the rendering begin!\n");
		//Sleep(2000);
	}
}