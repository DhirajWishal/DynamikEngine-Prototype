#pragma once

#ifndef _DYNAMIK_AUDIO_BASIC_AUDIO_CONTROLLER_H
#define _DYNAMIK_AUDIO_BASIC_AUDIO_CONTROLLER_H

#include "../AudioObjectController.h"
#include <iostream>
#include "Platform.h"

namespace Dynamik {
	namespace Audio {
		class DMK_API BasicAudioController :
			public AudioObjectController {
		public:

		public:
			BasicAudioController::BasicAudioController(std::string filePath);
			virtual bool BasicAudioController::shouldPlay();
		};
	}
}

#endif // !_DYNAMIK_AUDIO_BASIC_AUDIO_CONTROLLER_H
