#include "BackgroundAudioController.h"

namespace Dynamik {
	namespace Audio {
		bool BackgroundAudioController::shouldPlay() {
			return !isPaused;
		}
		BackgroundAudioController::BackgroundAudioController(std::string filepath) : AudioObjectController(filepath) {
			startPaused = false;
		}
	}
}