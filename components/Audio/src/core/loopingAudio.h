#pragma once

#include "audio_core.h"
#include <string>
#include <iostream>

namespace Dynamik {
	namespace audio {
		class AUDIO_API LoopingAudio : public AudioObject {
		public:

			LoopingAudio(std::string _filePath, int _duration);
			~LoopingAudio() {}
			std::string filePath;
			int duration;

			bool shouldPlay() override;
			void getResource() override;

			//TODO Delete
			void play() override;
		};
	}
}
