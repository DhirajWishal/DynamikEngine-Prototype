#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H
#define _DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H
#ifdef DMK_USE_VULKAN

namespace Dynamik {
	namespace ADGR {
		namespace core {
			inline B1 meshShadingSupported = false;
			inline B1 pushDescriptorsSupported = false;
			inline B1 checkpointsSupported = false;

			inline B1 VSYNC = true;

			//#define USE_SHADER_TESSELLATION
			//#define USE_SHADER_GEOMETRY

#define MAX_FRAMES_IN_FLIGHT 2
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_CONTROL_HEADER_H
