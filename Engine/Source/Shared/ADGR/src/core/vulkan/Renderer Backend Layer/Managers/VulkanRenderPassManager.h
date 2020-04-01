#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDER_PASS_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_RENDER_PASS_MANAGER_H

#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderPassInitInfo {
				B1 enableDepthAttachment = true;

				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;

				ARRAY<VkSubpassDescription> additionalSubPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			class VulkanRenderPassManager {
			public:
				VulkanRenderPassManager() {}
				~VulkanRenderPassManager() {}

				void initialize(POINTER<VulkanCoreObject> core, ADGRVulkanRenderPassInitInfo info);
				void terminate(POINTER<VulkanCoreObject> core);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_PASS_H
