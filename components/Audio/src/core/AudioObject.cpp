#pragma once
#include "AudioObject.h"
#include "log.h"

namespace Dynamik {
	namespace Audio {
		AudioObject::AudioObject(irrklang::ISoundEngine* audioEngine, AudioObjectController& _controller):
		audioEngine(audioEngine), controller(&_controller){
			if (!controller->is2D) {
				sound = audioEngine->play3D(controller->filepath.c_str(), controller->position, true, controller->startPaused, true);
				sound->setMaxDistance(1000000000.0f);
			}
			else {
				sound = audioEngine->play2D(controller->filepath.c_str(), true, controller->isPaused, true);
			}
		}
		
		void AudioObject::play() {
			sound->setPosition(controller->position);
			sound->setIsLooped(controller->isLooped);
			sound->setIsPaused(false);
		}

		void AudioObject::pause() {
			sound->setIsPaused(true);
		}

		void AudioObject::update()
		{
			if (controller->shouldPlay()) play();
			else pause();
		}
	}
}