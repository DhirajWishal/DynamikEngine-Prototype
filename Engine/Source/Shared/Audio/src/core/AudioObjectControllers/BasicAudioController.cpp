#pragma once

#include "BasicAudioController.h"

namespace Dynamik {
	namespace Audio {
		BasicAudioController::BasicAudioController(std::string filepath) : AudioObjectController(filepath) {
			isPaused = true;
			isLooped = false;
		}

		bool BasicAudioController::shouldPlay() {
			return !isPaused;
		}
	}
}