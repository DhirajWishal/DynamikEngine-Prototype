#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"

#include "VulkanShader.h"

#include <GameObject.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
			};

			struct ADGRVulkanPipelineLayoutInitInfo {
				// push constants info
				B1 pushConstantsEnable = false;									// Vulkan push constants enable
				UI32 pushConstantCount = 1;										// Vulkan push constants count
				I32 pushConstantOffset = 0;										// Vulkan push constants offset

				ARRAY<VkDescriptorSetLayout> additionalLayouts;
			};

			struct ADGRVulkanPipelineInitInfo {
				ARRAY<VkVertexInputBindingDescription> vertexBindingDescription;
				ARRAY<VkVertexInputAttributeDescription> vertexAttributeDescription;
				ARRAY<VulkanShader> shaders;

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

				// dynamic state info
				B1 dynamicStateEnable = false;									// Vulkan dynamic state enable
				VkPipelineDynamicStateCreateFlags dynamicStateFlags = 0;			// Vulkan dynamic state flags

				// pipeline info
				UI32 pipelineStageCount = 2;									// Vulkan pipeline stage count
				UI32 pipelineSubPass = 0;										// Vulkan pipeline sub pass
				VkPipeline pipelineBasePipelineHandle = VK_NULL_HANDLE;				// Vulkan base pipeline handle
				UI32 pipelineBasePipelineIndex = 0;
				VkPipelineCache pipelineCache = VK_NULL_HANDLE;					// Vulkan pipeline cache
				VkRenderPass renderPass = VK_NULL_HANDLE;
			};

			struct ADGRVulkanTextureInitInfo {
				std::string path;

				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

				VkFormat format;
				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			struct ADGRVulkanDescriptorPoolInitInfo {
				ARRAY<VkDescriptorPoolSize> additionalSizes;
			};

			struct ADGRVulkanDescriptorSetsInitInfo {
				ARRAY<VkWriteDescriptorSet> additionalWrites;
			};

			struct ADGRVulkanTextureContainer {
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkSampler imageSampler = VK_NULL_HANDLE;
				VkFormat format;
				UI32 mipLevels;

				UI32 width, height;
			};

			struct ADGRVulkanTextureSamplerInitInfo {
				VkFilter magFilter;
				VkFilter minFilter;
				VkSamplerAddressMode modeU;
				VkSamplerAddressMode modeV;
				VkSamplerAddressMode modeW;

				F32 minMipLevels;
				F32 maxMipLevels;

				VkBool32 anisotrophyEnable = VK_FALSE;
				F32 maxAnisotrophy = 16;
				VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				VkBool32 unnormalizedCoordinates = VK_FALSE;
				VkBool32 compareEnable = VK_FALSE;
				VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
				VkSamplerMipmapMode mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				F32 mipLoadBias = 0;
			};

			struct ADGRVulkanDescrpitorContainer {
				VkDescriptorSetLayout layout;
				ARRAY<VkDescriptorPool> descriptorPools;
				ARRAY<ARRAY<VkDescriptorSet>> descriptorSets;
			};

			class VulkanRenderableObject : public GameObject {
			public:
				VulkanRenderableObject(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkQueue presentQueue)
					: logicalDevice(logicalDevice), physicalDevice(physicalDevice), commandPool(commandPool), graphicsQueue(graphicsQueue), presentQueue(presentQueue) {}
				virtual ~VulkanRenderableObject() {}

				virtual void initializeDescriptorSetLayout(ADGRVulkanDescriptorSetLayoutInitInfo into);
				virtual void terminateDescriptorSetLayout();

				virtual void initializePipelineLayout(ADGRVulkanPipelineLayoutInitInfo info);
				virtual void terminatePipelineLayout();

				virtual void initializePipeline(VkExtent2D swapChainExtent, ADGRVulkanPipelineInitInfo info);
				virtual void terminatePipeline();

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos);
				virtual void initializeTextureSampler(ADGRVulkanTextureSamplerInitInfo info, POINTER<VkSampler> imageSampler);
				virtual void generateMipMaps(POINTER<ADGRVulkanTextureContainer> container);
				virtual void terminateTextures();

				virtual void initializeVertexBuffer(ARRAY<Vertex>* vertexes);
				virtual void initializeVertex2DBuffer(ARRAY<vertex2D>* vertexes);
				virtual void terminateVertexBuffer();

				virtual void initializeIndexBufferUI8(ARRAY<UI8>* indexes);
				virtual void initializeIndexBufferUI16(ARRAY<UI16>* indexes);
				virtual void initializeIndexBufferUI32(ARRAY<UI32>* indexes);
				virtual void initializeIndexBufferUI64(ARRAY<UI64>* indexes);
				virtual void terminateIndexBuffer();

				virtual void initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info);
				virtual void terminateDescriptorPool();

				virtual void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info);
				virtual void terminateDescriptorSets();

				virtual void initializeUniformBuffer(UI32 count);
				virtual void updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage);
				virtual void terminateUniformBuffer();

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				ARRAY<VkBuffer> vertexBuffers;
				ARRAY<VkDeviceMemory> vertexBufferMemories;
				UI32 vertexCount = 0;

				ARRAY<VkBuffer> indexBuffers;
				ARRAY<VkDeviceMemory> indexBufferMemories;
				UI32 indexCount = 0;
				UI32 indexbufferObjectTypeSize = 0;

				ARRAY<ADGRVulkanTextureContainer> textures;

				ADGRVulkanDescrpitorContainer descriptors;

				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
				VkPipeline pipeline = VK_NULL_HANDLE;

				ARRAY<VkBuffer> uniformBuffers;
				ARRAY<VkDeviceMemory> uniformBufferMemories;

				//VulkanPushConstantManager pushConstant;
				ARRAY<glm::vec4> pushConstants;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
