#include "adgrafx.h"
#include "VulkanShaderManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ADGRVulkanShaderContainer VulkanShaderManager::initialize(POINTER<VulkanCoreObject> core, ADGRVulkanShaderInitInfo info)
			{
				ADGRVulkanShaderContainer _container;

				if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_TESSELLATION)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_GEOMETRY)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				else if (_container.type == ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT)
					_container.stageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

				_container.stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				_container.stageCreateInfo.module = _container.shaderModule;
				_container.stageCreateInfo.pName = "main";
				_container.stageCreateInfo.pNext = VK_NULL_HANDLE;
				_container.stageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
				_container.stageCreateInfo.flags = VK_NULL_HANDLE;

				return _container;
			}

			void VulkanShaderManager::terminate(POINTER<VulkanCoreObject> core, ADGRVulkanShaderContainer container)
			{
				vkDestroyShaderModule(core->logicalDevice, container.shaderModule, nullptr);
			}

			VkShaderModule VulkanShaderManager::createModule(VkDevice device, ARRAY<CHR> code)
			{
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const UI32*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				return shaderModule;
			}

			ARRAY<CHR> VulkanShaderManager::getCode(std::string path)
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