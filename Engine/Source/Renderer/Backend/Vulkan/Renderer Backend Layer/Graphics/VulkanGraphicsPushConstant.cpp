#include "dmkafx.h"
#include "VulkanGraphicsPushConstant.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanGraphicsPushConstant::pushData(VkCommandBuffer buffer, VkPipelineLayout pipelineLayout)
			{
				vkCmdPushConstants(buffer, pipelineLayout, stageFlag, offset, byteSize, data);
			}
		}
	}
}