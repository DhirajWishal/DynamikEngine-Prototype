#include "audio_core.h"

#include "core/utils/dataStructures/linkedLists.h"

#include <iostream>
#include <string>

namespace Dynamik {
	namespace audio {

		void AudioCore::addAudioObject(AudioObject* audioObj) {
			audioQueue.push_back(audioObj);
		}

		void AudioCore::update()
		{
			for (auto itr = audioQueue.begin(); itr != audioQueue.end(); itr++) {
				if ((*itr)->shouldPlay()) {
					(*itr)->play();
				}
			}
		}

	}
}