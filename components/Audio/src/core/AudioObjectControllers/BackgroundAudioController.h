#pragma once
#include "../AudioObjectController.h"


namespace Dynamik {
	namespace Audio {
		class BackgroundAudioController :
			public AudioObjectController {
		public :

			BackgroundAudioController(std::string filepath);
			bool shouldPlay();
			bool is2D = true;
			bool startPaused = false;
		};
	}
}