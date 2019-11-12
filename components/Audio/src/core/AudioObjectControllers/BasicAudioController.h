#pragma once
#include "../AudioObjectController.h"
#include <iostream>

namespace Dynamik {
	namespace Audio {
		class BasicAudioController :
			public AudioObjectController {
		public:

		public :
			BasicAudioController::BasicAudioController(std::string filePath);
			virtual bool BasicAudioController::shouldPlay();
		};
	}
}