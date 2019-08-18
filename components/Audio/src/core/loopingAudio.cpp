
#include "loopingAudio.h"
#include "audio_core.h"


namespace Dynamik {
	namespace audio {
		
			bool LoopingAudio::shouldPlay() {
				return true;
			}
			void LoopingAudio::getResource() {
				std::cout << "file loaded" << std::endl;
			}

			void LoopingAudio::play()
			{
				std::cout << "Playing this file" + filePath  << "\n";
			}

	}
}

