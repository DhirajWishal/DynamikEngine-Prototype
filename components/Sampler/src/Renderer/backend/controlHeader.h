#pragma once

namespace Dynamik {
	namespace ADGR {
		namespace core {

			bool meshShadingSupported = false;
			bool pushDescriptorsSupported = true;
			bool checkpointsSupported = true;

			bool VSYNC = false;

//#define USE_SHADER_TESSELLATION	
//#define USE_SHADER_GEOMETRY	

#define MAX_FRAMES_IN_FLIGHT 2
		}
	}
}
