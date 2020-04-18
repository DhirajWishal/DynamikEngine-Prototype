#include "adgrafx.h"
#include "VulkanComputeShader.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeShader::initialize(VkDevice logicalDevice, std::string path)
			{
				code = getCode(path);

				shaderModule = createShaderModule(logicalDevice, code);
				stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
				stageCreateInfo.module = shaderModule;
				stageCreateInfo.pName = "main";
				stageCreateInfo.flags = VK_NULL_HANDLE;
				stageCreateInfo.pNext = nullptr;
				stageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
			}
			
			void VulkanComputeShader::terminate(VkDevice logicalDevice)
			{
				vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
			}
			
			VkShaderModule VulkanComputeShader::createShaderModule(VkDevice logicalDevice, ARRAY<CHR> code)
			{
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const UI32*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				return shaderModule;
			}
			
			ARRAY<CHR> VulkanComputeShader::getCode(std::string path)
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