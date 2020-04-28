#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_PRIMITIVE_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_PRIMITIVE_MANAGER_H

#include "../Common/VulkanPrimitives.h"
#include "VulkanGraphicsCore.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderPassInitInfo {
				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;
				B1 overrideDependencies = false;

				ARRAY<VkAttachmentDescription> attachments;

				ARRAY<VkSubpassDescription> subPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct ADGRVulkanGraphicsFrameBufferInitInfo {
				UI32 bufferCount = 0;
				VkExtent2D swapChainExtent;
				ARRAY<VkImageView> swapChainImageViews;

				ARRAY<VkImageView> attachments;
				VkRenderPass renderPass = VK_NULL_HANDLE;
			};

			struct ADGRVulkanTextureInitInfo {
				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			struct ADGRVulkanGraphicsPipelineInitInfo {
				/* Pipeline Layout information */
				// push constants info
				ARRAY<VkPushConstantRange> pushConstantRanges;
				ARRAY<VkDescriptorSetLayout> layouts;

				/* Pipeline information */
				ARRAY<VkVertexInputBindingDescription> vertexBindingDescription;
				ARRAY<VkVertexInputAttributeDescription> vertexAttributeDescription;
				ARRAY<VulkanGraphicsShader> shaders;
				VkExtent2D swapChainExtent;

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
				ARRAY<VkPipelineColorBlendAttachmentState> additionalColorBlendStates;
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

				// dynamic state info
				B1 dynamicStateEnable = false;									// Vulkan dynamic state enable
				VkPipelineDynamicStateCreateFlags dynamicStateFlags = 0;			// Vulkan dynamic state flags

				// pipeline info
				UI32 pipelineStageCount = 2;									// Vulkan pipeline stage count
				UI32 pipelineSubPass = 0;										// Vulkan pipeline sub pass
				VkPipeline pipelineBasePipelineHandle = VK_NULL_HANDLE;				// Vulkan base pipeline handle
				UI32 pipelineBasePipelineIndex = 0;
				VkRenderPass renderPass = VK_NULL_HANDLE;
				VkPipelineCache pipelineCache = VK_NULL_HANDLE;					// Vulkan pipeline cache
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			};

			/* Vulkan Graphics Primitive Manager
			 * This creates, updates and destroys the necessary rendering API primitives.
			 */
			class VulkanGraphicsPrimitiveManager {
				VulkanGraphicsPrimitiveManager() {}
				~VulkanGraphicsPrimitiveManager() {}

				static VulkanGraphicsPrimitiveManager instance;
			public:
				/* Basic initializations. These are mandatory to be called before calling any other. */
				static void setCoreData(POINTER<VulkanGraphicsCore> core, VkCommandPool cmdPool);
				static void changeCore(POINTER<VulkanGraphicsCore> newCore);
				static void changeCommandPool(VkCommandPool newCommandPool);

				/* Render Pass Functions */
				static VulkanRenderPass createRenderPass(ADGRVulkanRenderPassInitInfo info);
				static void destroyRenderPass(const VulkanRenderPass& renderPass);

				/* Frame Buffer Functions */
				static VulkanFrameBuffer createFrameBuffer(VkRenderPass renderPass, ARRAY<VkImageView> attachments, VkExtent2D extent, UI32 layerCount);
				static ARRAY<VulkanFrameBuffer> createFrameBuffers(ADGRVulkanGraphicsFrameBufferInitInfo info);
				static void destroyFrameBuffer(const ARRAY<VulkanFrameBuffer>& buffers);

				/* Swap Chain Functions */
				static VulkanSwapChain createSwapChain(UI32 width, UI32 height);
				static void createSwaChainImageView(POINTER<VulkanSwapChain> swapchain);

				/* Vertex Buffer Functions */
				static VulkanVertexBuffer createVertexBuffer(Mesh mesh, const ARRAY<DMKVertexAttribute>& attributes);
				static void destroyVertexBuffer(const VulkanVertexBuffer& buffer);

				/* Index Buffer Functions */
				static VulkanIndexBuffer createIndexBuffer(Mesh mesh, DMKDataType indexType);
				static void destroyIndexBuffer(const VulkanIndexBuffer& buffer);

				/* Texture Functions */
				static VulkanTextureData createTextureData(ADGRVulkanTextureInitInfo info, Texture texture);
				static void generateMipMaps(POINTER<VulkanTextureData> container);
				static void destroyTextureData(const VulkanTextureData& texture);

				/* Uniform Buffer Functions */
				static VulkanUniformBuffer createUniformBuffer(UI32 bufferSize);
				static void updateUniformBuffer(const VulkanUniformBuffer& buffer, UI32 bufferSize, DMKUniformBufferData bufferData);
				static void destroyUniformBuffer(const VulkanUniformBuffer& buffer);

				/* Pipeline Functions */
				static VulkanPipeline createPipeline(ADGRVulkanGraphicsPipelineInitInfo info);
				static void drstroyPipeline(const VulkanPipeline& pipeline);

			private:
				POINTER<VulkanGraphicsCore> myCore;
				VkCommandPool commandPool = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_GRAPHICS_PRIMITIVE_MANAGER_H
