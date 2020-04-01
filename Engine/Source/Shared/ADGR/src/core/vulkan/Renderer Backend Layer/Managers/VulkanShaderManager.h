#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SHADER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_SHADER_MANAGER_H

#include <string>
#include "Objects/VulkanCoreObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			enum class ADGRVulkanShaderType {
				ADGR_VULKAN_SHADER_TYPE_VERTEX,
				ADGR_VULKAN_SHADER_TYPE_TESSELLATION,
				ADGR_VULKAN_SHADER_TYPE_GEOMETRY,
				ADGR_VULKAN_SHADER_TYPE_FRAGMENT
			};

			struct ADGRVulkanShaderInitInfo {
				std::string path;
				ADGRVulkanShaderType type = ADGRVulkanShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
			};

			struct ADGRVulkanShaderContainer {
				ARRAY<CHR> code;
				VkShaderModule shaderModule = VK_NULL_HANDLE;
				ADGRVulkanShaderType type;
				VkPipelineShaderStageCreateInfo stageCreateInfo;
			};

			class VulkanShaderManager {
			public:
				VulkanShaderManager() {}
				~VulkanShaderManager() {}

				ADGRVulkanShaderContainer initialize(POINTER<VulkanCoreObject> core, ADGRVulkanShaderInitInfo info);
				void terminate(POINTER<VulkanCoreObject> core, ADGRVulkanShaderContainer container);

				static VkShaderModule createModule(VkDevice device, ARRAY<CHR> code);
				static ARRAY<CHR> getCode(std::string path);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_SHADER_H
