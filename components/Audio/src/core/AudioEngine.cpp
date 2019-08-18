#pragma once

#include "AudioEngine.h"

namespace Dynamik {
	namespace Audio {
			AudioEngine::AudioEngine() {
				irrklangEngine = irrklang::createIrrKlangDevice();

				if (!irrklangEngine)
					DMK_CORE_ERROR("Engine not started");
			}

			bool AudioEngine::addAudioController(AudioObjectController& controller)
			{
				return false;
			}
	}
}