#pragma once

#ifndef _DYNAMIK_AUDIO_BACKGROUND_AUDIO_CONTROLLER_H
#define _DYNAMIK_AUDIO_BACKGROUND_AUDIO_CONTROLLER_H

#include "../AudioObjectController.h"
#include "Platform.h"

namespace Dynamik {
	namespace Audio {
		class DMK_API BackgroundAudioController :
			public AudioObjectController {
		public:

			BackgroundAudioController(std::string filepath);
			bool shouldPlay();
			bool is2D = true;
			bool startPaused = false;
		};
	}
}

#endif // !_DYNAMIK_AUDIO_BACKGROUND_AUDIO_CONTROLLER_H
