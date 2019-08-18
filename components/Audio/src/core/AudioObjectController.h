#pragma once

#include <irrKlang.h>
#include <string>
namespace Dynamik {
	namespace Audio {
		class AudioObjectController {
		public :
			AudioObjectController(std::string filePath);
			std::string filepath;			
			virtual bool shouldPlay() = 0;
			bool isPaused = false;
			bool isLooped = false;
			bool is2D = false;
			bool startPaused = true;
			irrklang::vec3df position = { 0,0,0 };
		};
	}
}