#pragma once

#include "AudioEngine.h"
#include "AudioLog.h"
#include <iostream>

namespace Dynamik {
	namespace Audio {
		AudioEngine::AudioEngine() {
			irrklangEngine = irrklang::createIrrKlangDevice();

			//TODO
			//add a exception to handle irrKlang creation failure
			if (!irrklangEngine)
				LOG("Audio Engine didnt start");

			LOG("Audio engine started successfully");
		}

		bool AudioEngine::addAudioController(AudioObjectController& controller)
		{
			LOG("Controller for " + controller.filepath + " added");
			AudioObject audObject(irrklangEngine, controller);
			audioObjectList.push_back(audObject);
			return true;
		}
		void AudioEngine::update()
		{
			LOG("Audio Engine update loop started");
			for (auto itr = audioObjectList.begin(); itr != audioObjectList.end(); itr++) {
				if (&(itr) == nullptr) {
					audioObjectList.erase(itr);
					continue;
				}

				(itr)->update();
			}
		}
	}
}