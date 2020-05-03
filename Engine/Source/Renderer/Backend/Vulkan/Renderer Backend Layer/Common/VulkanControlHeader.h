#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_CONTROL_HEADER_H
#define _DYNAMIK_RENDERER_VULKAN_CONTROL_HEADER_H

#include "Public/datatypes.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			static B1 pushDescriptorsSupported = false;
			static B1 checkpointsSupported = false;
			static B1 meshShadingSupported = false;

			inline B1 VSYNC = true;

			//#define USE_SHADER_TESSELLATION
			//#define USE_SHADER_GEOMETRY

#define MAX_FRAMES_IN_FLIGHT 2
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_CONTROL_HEADER_H
