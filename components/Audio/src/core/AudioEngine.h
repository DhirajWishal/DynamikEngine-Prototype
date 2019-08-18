#pragma once


#include <irrKlang.h>
#include <core/log.h>
#include "AudioObjectController.h"

namespace Dynamik{
	 namespace Audio {
		 class AudioEngine {
			 public :

			 irrklang::ISoundEngine *irrklangEngine;
			 AudioEngine();
			 bool addAudioController(AudioObjectController &controller);
		 };
	}
}