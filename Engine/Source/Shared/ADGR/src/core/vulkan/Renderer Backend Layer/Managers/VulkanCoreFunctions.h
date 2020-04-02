#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_CORE_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_CORE_FUNCTIONS_H

#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanCoreObject.h"
#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanRenderObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanCoreObject;
			class VulkanRenderObject;

			struct ADGRVulkanInstanceInitInfo {
				std::string applicationName = "Dynamik Engine";
				std::string engineName = "Dynamik";
			};

			struct VulkanSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			struct ADGRVulkanRenderPassInitInfo {
				B1 enableDepthAttachment = true;

				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;

				ARRAY<VkSubpassDescription> additionalSubPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct ADGRVulkanPipelineInitInfo {
				std::string vertex;
				std::string fragment;

				// primitive assembly info
				VkPrimitiveTopology inputAssemblyTopology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;	// Vulkan input assembler topologies
				VkBool32 inputAssemblyPrimitiveRestartEnable = VK_FALSE;			// Vulkan primitive assembly restart enable

				// view port info
				UI32 viweportCount = 1;											// Vulkan view port count

				// scissor info
				UI32 scissorCount = 1;											// Vulkan scissor count
				ARRAY<VkOffset2D> offsets = { { 0, 0 } };						// Vulkan scissor offsets

				// rasterizer info
				VkBool32 rasterizerDepthClampEnable = VK_FALSE;						// Vulkan rasterizer depth clamp enable
				VkBool32 rasterizerDiscardEnable = VK_FALSE;						// Vulkan rasterizer discard enable
				VkPolygonMode rasterizerPolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;			// Vulkan rasterizer polygon mode
				F32 rasterizerLineWidth = 1.0f;									// Vulkan rasterizer line width
				VkCullModeFlagBits rasterizerCullMode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;		// Vulkan rasterizer cull mode
				VkFrontFace rasterizerFrontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;	// Vulkan rasterizer front face
				VkBool32 rasterizerDepthBiasEnable = VK_FALSE;						// Vulkan rasterizer depth bias enable

				// multi sampling info
				VkSampleCountFlagBits multisamplerMsaaSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;	// Vulkan multisampler MSAA samples
				VkBool32 multisamplerSampleShadingEnable = VK_FALSE;					// Vulkan multi sampler sample shading enable
				F32 multisamplerMinSampleShading = 0.2f;							// Vulkan multi sampler sample shading

				// depth stencil info
				VkBool32 depthStencilEnable = VK_TRUE;								// Vulkan depth stencil enable
				VkBool32 depthStencilWriteEnable = VK_TRUE;							// Vulkan depth stencil write enable
				VkCompareOp depthStencilCompareOp = VkCompareOp::VK_COMPARE_OP_LESS;	// Vulkan depth stencil compare op
				VkBool32 depthStencilBoundsTestEnable = VK_FALSE;					// Vulkan depth stencil bounds test enable
				VkBool32 depthStencilTestEnable = VK_FALSE;							// Vulkan depth stencil test enable

				// color blender info
				ARRAY<VkColorComponentFlags> colorBlenderColorWriteMasks = {	// Vulkan color blend write masks
					VK_COLOR_COMPONENT_R_BIT	// Red
					| VK_COLOR_COMPONENT_G_BIT	// Green
					| VK_COLOR_COMPONENT_B_BIT	// Blue
					| VK_COLOR_COMPONENT_A_BIT	// Alpha
				};
				ARRAY<VkBool32> colorBlenderBlendEnables = { VK_FALSE };		// Vulkan color blend blend enables

				VkBool32 colorBlendingLogicOpEnable = VK_FALSE;						// Vulkan color blend blending logical op enable
				VkLogicOp colorBlendingLogicOp = VkLogicOp::VK_LOGIC_OP_COPY;		// Vulkan color blend blending logical op
				UI32 colorBlendingColorBlendCount = 1;							// Vulkan color blend blending count
				ARRAY<F32> colorBlendingBlendConstants = {					// Vulkan color blend constants
					0.0f,	// Red
					0.0f,	// Green
					0.0f,	// Blue
					0.0f	// Alpha
				};

				// push constants info
				B1 pushConstantsEnable = false;									// Vulkan push constants enable
				UI32 pushConstantCount = 1;										// Vulkan push constants count
				I32 pushConstantOffset = 0;										// Vulkan push constants offset

				// dynamic state info
				B1 dynamicStateEnable = false;									// Vulkan dynamic state enable
				VkPipelineDynamicStateCreateFlags dynamicStateFlags = 0;			// Vulkan dynamic state flags

				// pipeline info
				UI32 pipelineStageCount = 2;									// Vulkan pipeline stage count
				UI32 pipelineSubPass = 0;										// Vulkan pipeline sub pass
				VkPipeline pipelineBasePipelineHandle = VK_NULL_HANDLE;				// Vulkan base pipeline handle
				UI32 pipelineBasePipelineIndex = 0;
			};

			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
			};

			struct ADGRVulkanDescriptorPoolInitInfo {
				ARRAY<VkDescriptorPoolSize> additionalSizes;
				UI32 poolCount = 3;
			};

			struct ADGRVulkanDescriptorSetsInitInfo {
				ARRAY<VkWriteDescriptorSet> additionalWrites;
			};

			struct ADGRVulkanDescrpitorContainer {
				VkDescriptorSetLayout layout;
				ARRAY<VkDescriptorPool> descriptorPools;
				ARRAY<ARRAY<VkDescriptorSet>> descriptorSets;
			};

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

			struct ADGRVulkanSyncObjectsContainer {
				ARRAY<VkSemaphore> imageAvailables;
				ARRAY<VkSemaphore> renderFinishes;
				ARRAY<VkFence> inFlightFences;
			};

			class VulkanCoreFunctions {
				VulkanCoreFunctions() {}
				VulkanCoreFunctions(const VulkanCoreFunctions&) = delete;
				VulkanCoreFunctions operator=(const VulkanCoreFunctions&) = delete;
				VulkanCoreFunctions operator=(VulkanCoreFunctions&&) = delete;

			public:
				static void initializeInstance(POINTER<VulkanCoreObject> core, ADGRVulkanInstanceInitInfo info);
				static void terminateInstance(POINTER<VulkanCoreObject> core);

				static void initializeDebugger(POINTER<VulkanCoreObject> core);
				static void terminateDebugger(POINTER<VulkanCoreObject> core);
				static void populateDebugMessegerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

				static void initializeDevice(POINTER<VulkanCoreObject> core);
				static void terminateDevice(POINTER<VulkanCoreObject> core);
				static void initPhysicalDevice(POINTER<VulkanCoreObject> core);
				static void initLogicalDevice(POINTER<VulkanCoreObject> core);

				static void initializeSwapChain(POINTER<VulkanCoreObject> core, UI32 width, UI32 height);
				static void terminateSwapChain(POINTER<VulkanCoreObject> core);
				static void initializeSwapChainImageViews(POINTER<VulkanCoreObject> core);
				static VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);

				static void initializeRenderPass(POINTER<VulkanCoreObject> core, ADGRVulkanRenderPassInitInfo info);
				static void terminateRenderPass(POINTER<VulkanCoreObject> core);

				static void initializePipeline(POINTER<VulkanCoreObject> core, ADGRVulkanPipelineInitInfo info, POINTER<VulkanRenderObject> object);
				static void terminatePipeline(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				static void initializeDescriptorLayout(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetLayoutInitInfo info, POINTER<VulkanRenderObject> object);
				static void terminateDescriptorLayout(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				static void initializeDescriptorPool(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorPoolInitInfo info, POINTER<VulkanRenderObject> object);
				static void terminateDescriptorPool(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				static void initializeDescriptorSets(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetsInitInfo info, POINTER<VulkanRenderObject> object);
				static void terminateDescriptorSets(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				static ADGRVulkanShaderContainer initializeShader(POINTER<VulkanCoreObject> core, ADGRVulkanShaderInitInfo info);
				static void terminateShader(POINTER<VulkanCoreObject> core, ADGRVulkanShaderContainer container);

				static VkShaderModule createShaderModule(VkDevice device, ARRAY<CHR> code);
				static ARRAY<CHR> getCode(std::string path);

				static void initializeSyncObjects(VkDevice device, POINTER<ADGRVulkanSyncObjectsContainer> container);
				static void terminateSyncObjects(VkDevice device, POINTER<ADGRVulkanSyncObjectsContainer> container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_CORE_FUNCTIONS_H
