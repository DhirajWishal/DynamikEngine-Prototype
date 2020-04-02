#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_MANAGER_H

#include "Objects/VulkanCoreObject.h"
#include "Objects/VulkanRenderObject.h"
#include "UniformBufferObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanRenderObject;

			class VulkanUniformBufferManager {
			public:
				VulkanUniformBufferManager() {}
				~VulkanUniformBufferManager() {}

				void initialize(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);
				void update(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object, UniformBufferObject uniformBuferObject, UI32 currentImage);
				void terminate(POINTER<VulkanCoreObject> core, VulkanRenderObject object);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_UNIFORM_BUFFER_H
