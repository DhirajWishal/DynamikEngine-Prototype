#include "dmkafx.h"
#include "VulkanAnimationManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanAnimationManager::addInitializedAnimation(VkDevice logicalDevice, POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples, VulkanGraphicsRenderableObject* animation)
			{
			}

			void VulkanAnimationManager::addUninitializedAnimation(VulkanGraphicsRenderableObject* animation)
			{
			}

			void VulkanAnimationManager::addSkeletalAnimation(const VulkanSkeletalAnimation& animation)
			{
				mySkeletalAnimations.pushBack(animation);
			}
		}
	}
}