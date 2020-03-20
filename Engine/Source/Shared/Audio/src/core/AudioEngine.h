#pragma once

#ifndef _DYNAMIK_AUDIO_ENGINE_H
#define _DYNAMIK_AUDIO_ENGINE_H

#include <irrKlang.h>
#include "AudioObject.h"
#include "AudioObjectController.h"
#include <list>
#include <Platform.h>

namespace Dynamik {
	namespace Audio {
		class DMK_API AudioEngine {
		private:
			irrklang::ISoundEngine* irrklangEngine;
			std::list<AudioObject> audioObjectList;

		public:
			AudioEngine();
			void Initialize();

			bool addAudioController(AudioObjectController& controller);
			void update();
		};
	}
}

#endif // !_DYNAMIK_AUDIO_ENGINE_H
