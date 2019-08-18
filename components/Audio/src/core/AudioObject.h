#pragma once

#include <irrKlang.h>
#include "AudioObjectController.h"

namespace Dynamik {
	namespace Audio {
		irrklang::ISoundEngine* audioEngine;
		AudioObjectController* controller;

		class AudioObject {
			AudioObject(irrklang::ISoundEngine* audioEngine, AudioObjectController& _controller);
		};
	}
}