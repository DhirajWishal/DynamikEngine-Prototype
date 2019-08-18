#pragma once

#include "BasicAudioController.h"

namespace Dynamik {
	namespace Audio {
		BasicAudioController::BasicAudioController(std::string filepath) : AudioObjectController(filepath) {
			
		}

		bool BasicAudioController::shouldPlay() {
			return !isPaused;
		}
	}
}