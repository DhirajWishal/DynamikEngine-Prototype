#include "adgrafx.h"
#include "shaderManager.h"

#include "controlHeader.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		namespace core {
			void shaderManager::loadShader(DMK_ShaderCode shaderCode, ShaderType type) {
			}

			void shaderManager::init(ADGRVulkanDataContainer* container, ADGRVulkanShaderDataContainer* shaderDataContainer) {
				ADGRVulkanPipelineDataContainer pipelineContainer = {};

				VkPipelineShaderStageCreateInfo _shaderStageCreateInfo;
				VkShaderModule _shaderModule;

				for (auto shaderCode : shaderDataContainer->shaderCodes) {
					// initialize the shader modules
					if (shaderCode.shaderType == DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX) {
						VkShaderModule vertexShaderModule = createShaderModule(container->device, shaderCode.shaderCode);
						_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
						_shaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
						_shaderStageCreateInfo.module = vertexShaderModule;
						_shaderStageCreateInfo.pName = "main";

						_shaderModule = vertexShaderModule;
					}
					else if (shaderCode.shaderType == DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION) {
						VkShaderModule tessellationShaderModule = createShaderModule(container->device, shaderCode.shaderCode);
						_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
						_shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
						_shaderStageCreateInfo.module = tessellationShaderModule;
						_shaderStageCreateInfo.pName = "main";

						_shaderModule = tessellationShaderModule;
					}
					else if (shaderCode.shaderType == DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY) {
						VkShaderModule geometryShaderModule = createShaderModule(container->device, shaderCode.shaderCode);
						_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
						_shaderStageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
						_shaderStageCreateInfo.module = geometryShaderModule;
						_shaderStageCreateInfo.pName = "main";

						_shaderModule = geometryShaderModule;
					}
					else if (shaderCode.shaderType == DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT) {
						VkShaderModule fragmentShaderModule = createShaderModule(container->device, shaderCode.shaderCode);
						_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
						_shaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
						_shaderStageCreateInfo.module = fragmentShaderModule;
						_shaderStageCreateInfo.pName = "main";

						_shaderModule = fragmentShaderModule;
					}

					shaderDataContainer->shaderStageInfo.push_back(_shaderStageCreateInfo);
					shaderDataContainer->shaderModules.push_back(_shaderModule);
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