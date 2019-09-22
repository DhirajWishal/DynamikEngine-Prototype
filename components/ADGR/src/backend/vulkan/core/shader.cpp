#include "adgrafx.h"
#include "shader.h"

#include "controlHeader.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void shaderManager::loadShader(DMK_ShaderCode shaderCode, ShaderType type) {
				switch (type) {
				case Dynamik::ADGR::core::VERTEX_SHADER:
					vertexShaderCode = shaderCode;
					break;
#ifdef USE_SHADER_TESSELLATION
				case Dynamik::ADGR::core::TESSELLATION_SHADER:
					tessellationShaderCode = shaderCode;
					break;
#endif
#ifdef USE_SHADER_GEOMETRY
				case Dynamik::ADGR::core::GEOMETRY_SHADER:
					geometryShaderCode = shaderCode;
					break;
#endif
				case Dynamik::ADGR::core::FRAGMENT_SHADER:
					fragmentShaderCode = shaderCode;
					break;
				default:
					break;
				}
			}

			void shaderManager::init() {
				// initialize the shader modules
				*m_vertexShaderModule = createShaderModule(*m_vertexShaderCode);

				// initialize the vertex shader module
				vertShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vertShaderStageInfo->stage = VK_SHADER_STAGE_VERTEX_BIT;
				vertShaderStageInfo->module = *m_vertexShaderModule;
				vertShaderStageInfo->pName = "main";

				*m_fragmentShaderModule = createShaderModule(*m_fragmentShaderCode);

				// initialize the fragment shader module
				fragShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				fragShaderStageInfo->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				fragShaderStageInfo->module = *m_fragmentShaderModule;
				fragShaderStageInfo->pName = "main";

#ifdef USE_SHADER_TESSELLATION
				* m_tessShaderModule = createShaderModule(*m_tessallationShaderCode);

				tessShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				tessShaderStageInfo->stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				tessShaderStageInfo->module = *m_tessShaderModule;
				tessShaderStageInfo->pName = "main";
#endif

#ifdef USE_SHADER_GEOMETRY
				* m_geoShaderModule = createShaderModule(*m_geometryShaderCode);

				geoShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				geoShaderStageInfo->stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				geoShaderStageInfo->module = *m_geoShaderModule;
				geoShaderStageInfo->pName = "main";
#endif

				VkPipelineShaderStageCreateInfo shaderStages[] = {
					*vertShaderStageInfo,
					*fragShaderStageInfo
				};
			}

			VkShaderModule shaderManager::createShaderModule(DMK_ShaderCode& code) {
				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = code.size();
				createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

				VkShaderModule shaderModule;
				if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
					DMK_CORE_FATAL("Failed to create Shader module!");

				return shaderModule;
			}

			void shaderManager::deleteShaders() {
				vkDestroyShaderModule(*m_device, *m_vertexShaderModule, nullptr);
				vkDestroyShaderModule(*m_device, *m_fragmentShaderModule, nullptr);
#ifdef USE_SHADER_TESSELLATION
				vkDestroyShaderModule(*m_device, *m_vtessellationShaderModule, nullptr);
#endif
#ifdef USE_SHADER_GEOMETRY
				vkDestroyShaderModule(*m_device, *m_geometryShaderModule, nullptr);
#endif
			}

			void shaderManager::compileShaders(std::string path, bool activate) {
				if (activate) {
					bool check = path.find(".conf") != std::string::npos
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
