#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"

#include "VulkanCore.h"
#include "VulkanShader.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderLayout.h"

#include <GameObject.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanRenderableObjectInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};

			struct ADGRVulkanShaderPathContainer {
				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";
			};

			struct ADGRVulkanPipelineInitInfo {
				ARRAY<VkVertexInputBindingDescription> vertexBindingDescription;
				ARRAY<VkVertexInputAttributeDescription> vertexAttributeDescription;
				ARRAY<VulkanShader> shaders;
				B1 isTexturesAvailable = true;

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

			struct ADGRVulkanMaterialDescriptor {
				struct PushBlock {
					F32 roughness;
					F32 metallic;
					F32 r, g, b;
				} params;

				std::string name;

				ADGRVulkanMaterialDescriptor() {}
				ADGRVulkanMaterialDescriptor(std::string n, glm::vec3 c, F32 r, F32 m) : name(n) {
					params.roughness = r;
					params.metallic = m;
					params.r = c.r;
					params.g = c.g;
					params.b = c.b;
				}
				~ADGRVulkanMaterialDescriptor() {}
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

			struct ADGRVulkanDescrpitorContainer {
				VkDescriptorSetLayout layout = VK_NULL_HANDLE;
				VkDescriptorPool pool = VK_NULL_HANDLE;
				VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
			};

			struct ADGRVulkanUnformBufferContainer {
				ARRAY<VkBuffer> buffers;
				ARRAY<VkDeviceMemory> bufferMemories;
			};

			struct ADGRVulkanRenderData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

				POINTER<VulkanSwapChain> swapChainPointer;

				ARRAY<VkBuffer> vertexBuffers;
				ARRAY<VkDeviceMemory> vertexBufferMemories;
				UI32 vertexCount = 0;

				ARRAY<VkBuffer> indexBuffers;
				ARRAY<VkDeviceMemory> indexBufferMemories;
				UI32 indexCount = 0;
				UI32 indexbufferObjectTypeSize = 0;

				ARRAY<ADGRVulkanTextureContainer> textures;

				ADGRVulkanDescrpitorContainer descriptors;

				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

				ARRAY<ADGRVulkanUnformBufferContainer> uniformBufferContainers;

				ADGRVulkanMaterialDescriptor materialDescriptor;
				B1 enableMaterials = false;
			};

			struct StaggingBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct ADGRVulkan3DObjectData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
				std::string modelpath = "";
				std::string materialName = "Metal";
				B1 isPBR = false;

				POINTER<ARRAY<ARRAY<Vertex>>> vertexBufferObjects;
				POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				ARRAY<std::string> texturePaths;

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
			};

			class VulkanRenderableObject : public GameObject {
			public:
				VulkanRenderableObject() {}
				VulkanRenderableObject(ADGRVulkanRenderableObjectInitInfo info)
					: logicalDevice(info.logicalDevice), physicalDevice(info.physicalDevice), commandPool(info.commandPool), graphicsQueue(info.graphicsQueue), presentQueue(info.presentQueue) {}
				virtual ~VulkanRenderableObject() {}

				virtual void initializeResources(ADGRVulkanRenderableObjectInitInfo info);

				virtual ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples);

				virtual void initializePipeline(ADGRVulkanPipelineInitInfo info);
				virtual void terminatePipeline();

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos);
				virtual void generateMipMaps(POINTER<ADGRVulkanTextureContainer> container);
				virtual void terminateTextures();

				virtual void initializeVertexBuffer(ARRAY<Vertex>* vertexes);
				virtual void initializeVertex2DBuffer(ARRAY<vertex2D>* vertexes);
				virtual void initializeVertexBufferP(ARRAY<VertexP>* vertexes);
				virtual void initializeVertexBufferPN(ARRAY<VertexPN>* vertexes);
				virtual void terminateVertexBuffer();

				virtual void initializeIndexBufferUI8(ARRAY<UI8>* indexes);
				virtual void initializeIndexBufferUI16(ARRAY<UI16>* indexes);
				virtual void initializeIndexBufferUI32(ARRAY<UI32>* indexes);
				virtual void initializeIndexBufferUI64(ARRAY<UI64>* indexes);
				virtual void terminateIndexBuffer();

				virtual void initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info);
				virtual void terminateDescriptorPool();

				virtual void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info);

				virtual void initializeUniformBuffer();
				virtual void updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage);
				virtual void terminateUniformBuffer();

				void setSwapChainContainer(POINTER<VulkanSwapChain> swapChain) { myRenderData.swapChainPointer = swapChain; }
				void setRenderData(ADGRVulkanRenderData data) { myRenderData = data; }
				const ADGRVulkanRenderData getRenderData() const { return myRenderData; }
				ADGRVulkanRenderData myRenderData;

			protected:
				virtual void initializeNoTextureDescriptorSetLayout();
				virtual void initializeNoTexturePipelineLayout();

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkDescriptorSetLayout noTextureDescriptorSetLayout = VK_NULL_HANDLE;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
