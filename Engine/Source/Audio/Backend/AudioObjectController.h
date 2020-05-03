#pragma once

#ifndef _DYNAMIK_AUDIO_OBJECT_CONTROLLER_H
#define _DYNAMIK_AUDIO_OBJECT_CONTROLLER_H

#include <irrKlang.h>
#include <string>
#include "core.h"

namespace Dynamik {
	namespace Audio {
		class DMK_API AudioObjectController {
		public:
			AudioObjectController(std::string filePath);
			std::string filepath;
			virtual bool shouldPlay() = 0;
			bool isPaused = false;
			bool isLooped = false;
			bool is2D = false;
			bool startPaused = true;
			bool resetPlayer = false;
			irrklang::vec3df position = { 0,0,0 };
		};
	}
}

#endif // !_DYNAMIK_AUDIO_OBJECT_CONTROLLER_H
