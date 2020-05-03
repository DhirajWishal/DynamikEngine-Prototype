#include "dmkafx.h"
#include "VulkanGraphicsShader.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanGraphicsShader::initialize(VkDevice logicalDevice, VulkanGraphicsShaderInitInfo info)
			{
				type = info.type;
				code = getCode(info.path);

				shaderModule = createShaderModule(logicalDevice, code);

				if (type == VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX)
					stageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				else if (type == VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION)
					stageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				else if (type == VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY)
					stageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				else if (type == VulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT)
					stageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

				stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageCreateInfo.module = shaderModule;
				stageCreateInfo.pName = "main";
				stageCreateInfo.flags = VK_NULL_HANDLE;
				stageCreateInfo.pNext = nullptr;
				stageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
			}

			void VulkanGraphicsShader::terminate(VkDevice logicalDevice)
			{
				vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
			}

			VkShaderModule VulkanGraphicsShader::createShaderModule(VkDevice logicalDevice, ARRAY<CHR> code)
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

			ARRAY<CHR> VulkanGraphicsShader::getCode(std::string path)
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