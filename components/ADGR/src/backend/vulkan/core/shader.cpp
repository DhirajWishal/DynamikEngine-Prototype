#include "adgrafx.h"
#ifdef DMK_USE_VULKAN
#include "shader.h"

#include "controlHeader.h"
#include "..\..\opengl\core\shader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			void shaderManager::loadShader(DMK_ShaderCode shaderCode, ShaderType type) {
			}

			void shaderManager::init(ADGRVulkanDataContainer* container, ADGRVulkanShaderDataContainer* shaderDataContainer) {
				ADGRVulkanPipelineDataContainer pipelineContainer = {};

				// initialize the shader modules
				if (shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX].size()) {
					VkShaderModule vertexShaderModule = createShaderModule(container->device, shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX]);
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX].stage = VK_SHADER_STAGE_VERTEX_BIT;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX].module = vertexShaderModule;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX].pName = "main";

					shaderDataContainer->shaderModules[DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX] = vertexShaderModule;
				}
				if (shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION].size()) {
					VkShaderModule tessellationShaderModule = createShaderModule(container->device, shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION]);
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION].module = tessellationShaderModule;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION].pName = "main";

					shaderDataContainer->shaderModules[DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION] = tessellationShaderModule;
				}
				if (shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY].size()) {
					VkShaderModule geometryShaderModule = createShaderModule(container->device, shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY]);
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY].module = geometryShaderModule;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY].pName = "main";

					shaderDataContainer->shaderModules[DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY] = geometryShaderModule;
				}
				if (shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT].size()) {
					VkShaderModule fragmentShaderModule = createShaderModule(container->device, shaderDataContainer->shaderCodes[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT]);
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT].module = fragmentShaderModule;
					shaderDataContainer->shaderStageInfo[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT].pName = "main";

					shaderDataContainer->shaderModules[DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT] = fragmentShaderModule;
				}
			}

			VkShaderModule shaderManager::createShaderModule(VkDevice device, DMK_ShaderCode& code) {
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const UI32*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				return shaderModule;
			}

			void shaderManager::deleteShaders(ADGRVulkanDataContainer& container, ADGRVulkanPipelineDataContainer& pipelineContainer) {
				for (I32 i = 0; i < 4; i++)
					if (pipelineContainer.shaderModules[i] != VK_NULL_HANDLE)
						vkDestroyShaderModule(container.device, pipelineContainer.shaderModules[i], nullptr);
			}

			void shaderManager::compileShaders(std::string path, B1 activate) {
				if (activate) {
					B1 check = path.find(".conf") != std::string::npos
						|| path.find(".vert") != std::string::npos
						|| path.find(".tesc") != std::string::npos
						|| path.find(".tese") != std::string::npos
						|| path.find(".geom") != std::string::npos
						|| path.find(".frag") != std::string::npos
						|| path.find(".comp") != std::string::npos
						|| path.find(".mesh") != std::string::npos
						|| path.find(".task") != std::string::npos
						|| path.find(".rgen") != std::string::npos
						|| path.find(".rint") != std::string::npos
						|| path.find(".rahit") != std::string::npos
						|| path.find(".rchit") != std::string::npos
						|| path.find(".rmiss") != std::string::npos
						|| path.find(".rcall") != std::string::npos
						|| path.find(".glsl") != std::string::npos
						|| path.find(".hlsl") != std::string::npos;

					if (check) {
						command = shaderCompilerPath + " -V \"" + path + "\"";
						system(command.c_str());
					}
					else {
						DMK_CORE_ERROR("Invalid shader type!");
					}
				}
			}
		}
	}
}
#endif // !DMK_USE_VULKAN
