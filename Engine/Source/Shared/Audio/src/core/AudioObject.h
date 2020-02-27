#pragma once

#ifndef _DYNAMIK_AUDIO_OBJECT_H
#define _DYNAMIK_AUDIO_OBJECT_H

#include <irrKlang.h>
#include "AudioObjectController.h"
#include <Platform.h>

namespace Dynamik {
	namespace Audio {
		class DMK_API AudioObject {
			irrklang::ISoundEngine* audioEngine;
			AudioObjectController* controller;
			irrklang::ISound* sound;

		public:
			AudioObject(irrklang::ISoundEngine* audioEngine, AudioObjectController& _controller);
			void update();
		private:
			void play();
			void pause();
			bool isFinished();
		};
	}
}

#endif // !_DYNAMIK_AUDIO_OBJECT_H
