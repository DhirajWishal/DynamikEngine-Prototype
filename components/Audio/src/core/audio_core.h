#pragma once

#pragma once

#include "interface.h"

#include <vector>
#include <list>

namespace Dynamik {
	namespace audio {

		class AUDIO_API AudioObject {
		public:
			virtual void play() = 0;

			virtual bool shouldPlay() = 0;

			//The next time this audio is loaded 
			virtual long long nextPlayTime() = 0;

			//Gets the resource needed for playing
			virtual void getResource() = 0;
		};

		class AUDIO_API AudioCore {
		public:
			std::list<AudioObject*> audioQueue;
			void addAudioObject(AudioObject *audioObj);


			//Called on each game loop
			void update();
		};
	}
}