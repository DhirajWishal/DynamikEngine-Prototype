#include "adgrafx.h"
#include "VulkanRenderLayout.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VkDescriptorSetLayout VulkanRenderLayout::createDescriptorSetLayout(VkDevice device, ADGRVulkanDescriptorSetLayoutInitInfo info)
			{
				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				if (!info.overrideBindings)
				{
					VkDescriptorSetLayoutBinding uboLayoutBinding = {};
					uboLayoutBinding.binding = 0; // info.bindIndex;
					uboLayoutBinding.descriptorCount = 1;
					uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
					uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
					bindings.push_back(uboLayoutBinding);

					VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
					samplerLayoutBinding.binding = 1; // info.bindIndex;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
					samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					bindings.push_back(samplerLayoutBinding);
				}

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.push_back(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				VkDescriptorSetLayout _layout = VK_NULL_HANDLE;
				if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");

				return _layout;
			}
			
			void VulkanRenderLayout::terminateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout)
			{
				vkDestroyDescriptorSetLayout(device, layout, nullptr);
			}
			
			VkPipelineLayout VulkanRenderLayout::createPipelineLayout(VkDevice device, ADGRVulkanPipelineLayoutInitInfo info)
			{
				// initialize the pipeline layout
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = info.layouts.size();
				pipelineLayoutInfo.pSetLayouts = info.layouts.data();

				if (info.pushConstantRanges.size())
				{
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantRanges.size();
					pipelineLayoutInfo.pPushConstantRanges = info.pushConstantRanges.data();
				}
				else
				{
					pipelineLayoutInfo.pushConstantRangeCount = 0;
					pipelineLayoutInfo.pPushConstantRanges = VK_NULL_HANDLE;
				}

				// create the pipeline layout
				VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
				if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				return _pipelineLayout;
			}
			
			void VulkanRenderLayout::terminatePipelineLayout(VkDevice device, VkPipelineLayout layout)
			{
				vkDestroyPipelineLayout(device, layout, nullptr);
			}
		}
	}
}