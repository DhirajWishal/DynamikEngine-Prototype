#include "adgrafx.h"
#include "VulkanPushConstant.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanPushConstant::pushData(VkCommandBuffer buffer, VkPipelineLayout pipelineLayout)
			{
				vkCmdPushConstants(buffer, pipelineLayout, stageFlag, offset, byteSize, data);
			}
		}
	}
}