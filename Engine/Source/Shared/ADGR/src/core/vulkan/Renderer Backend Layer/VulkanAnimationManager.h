#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_ANIMATION_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_ANIMATION_MANAGER_H

#include "Animations/VulkanSkeletalAnimation.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanAnimationManager {
			public:
				VulkanAnimationManager() {}
				~VulkanAnimationManager() {}

				void addInitializedAnimation(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples, VulkanGraphicsRenderableObject* animation);
				void addUninitializedAnimation(VulkanGraphicsRenderableObject* animation);

				void addSkeletalAnimation(const VulkanSkeletalAnimation& animation);

			private:
				ARRAY<VulkanSkeletalAnimation> mySkeletalAnimations;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_ANIMATION_MANAGER_H
