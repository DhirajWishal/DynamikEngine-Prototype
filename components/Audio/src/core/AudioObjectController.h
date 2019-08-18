#pragma once

#include <string>
namespace Dynamik {
	namespace Audio {
		class AudioObjectController {
		public :
			std::string filepath;
			AudioObjectController(std::string& filePath);
		};
	}
}