#include "dmkafx.h"
#include "VulkanGraphicsAttachment.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsAttachment::_initializeDescriptorLayout(UI32 destination, VkDescriptorType descriptorType, VkShaderStageFlags shaderStage)
			{
				/* Initialize Descriptor Set Layout */
				VkDescriptorSetLayoutBinding binding = {};
				binding.binding = destination;
				binding.descriptorCount = 1;
				binding.descriptorType = descriptorType;
				binding.pImmutableSamplers = nullptr; // Optional
				binding.stageFlags = shaderStage;

				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.bindings = { binding };
				descriptor.initializeLayout(logicalDevice, layoutInitInfo);
			}
		}
	}
}