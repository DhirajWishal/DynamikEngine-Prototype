#pragma once


#include <irrKlang.h>
#include "AudioObject.h"
#include "AudioObjectController.h"
#include <list>

namespace Dynamik{
	 namespace Audio {
		 class AudioEngine {
		 private :
			 irrklang::ISoundEngine* irrklangEngine;
			 std::list<AudioObject> audioObjectList;

			 public :			 
			 AudioEngine();
			 bool addAudioController(AudioObjectController &controller);
			 void update();
		 };
	}
}