#include "adgrafx.h"
#include "VulkanShader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanShader::initialize(ADGRVulkanShaderInitInfo info)
			{
				type = info.type;

				code = getCode(info.path);

				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const UI32*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(info.device.logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				if (type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX)
				{
					stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					stageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
					stageCreateInfo.module = shaderModule;
					stageCreateInfo.pName = "main";
				}
				else if (type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION)
				{
					stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					stageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					stageCreateInfo.module = shaderModule;
					stageCreateInfo.pName = "main";
				}
				else if (type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY)
				{
					stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					stageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
					stageCreateInfo.module = shaderModule;
					stageCreateInfo.pName = "main";
				}
				else if (type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT)
				{
					stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					stageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					stageCreateInfo.module = shaderModule;
					stageCreateInfo.pName = "main";
				}
			}

			void VulkanShader::terminate(VulkanDevice device)
			{
				vkDestroyShaderModule(device.logicalDevice, shaderModule, nullptr);
			}
			
			ARRAY<CHR> VulkanShader::getCode(std::string path)
			{
				std::ifstream file(path, std::ios::ate | std::ios::binary);

				if (!file.is_open())
					throw std::runtime_error("Failed to Open file!");

				size_t fileSize = (size_t)file.tellg();
				ARRAY<char> buffer(fileSize);
				file.seekg(0);
				file.read(buffer.data(), fileSize);

				file.close();
				return buffer;
			}
		}
	}
}