#include "adgrafx.h"
#include "VulkanAnimationManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanAnimationManager::addInitializedAnimation(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples, VulkanRenderableObject* animation)
			{
			}

			void VulkanAnimationManager::addUninitializedAnimation(VulkanRenderableObject* animation)
			{
			}

			void VulkanAnimationManager::addSkeletalAnimation(const VulkanSkeletalAnimation& animation)
			{
				mySkeletalAnimations.pushBack(animation);
			}
		}
	}
}