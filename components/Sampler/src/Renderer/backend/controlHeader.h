#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			inline bool meshShadingSupported = false;
			inline bool pushDescriptorsSupported = false;
			inline bool checkpointsSupported = false;

			inline bool VSYNC = false;

//#define USE_SHADER_TESSELLATION	
//#define USE_SHADER_GEOMETRY	

#define MAX_FRAMES_IN_FLIGHT 2
		}
	}
}
