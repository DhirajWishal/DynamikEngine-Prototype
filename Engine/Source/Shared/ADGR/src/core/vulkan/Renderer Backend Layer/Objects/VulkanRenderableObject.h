#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"

#include "VulkanCore.h"
#include "VulkanShader.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderLayout.h"
#include "VulkanPipeline.h"
#include "VulkanPushConstant.h"

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

			struct ADGRVulkanPiplineContainer {
				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout layout = VK_NULL_HANDLE;
			};

			struct ADGRVulkanRenderData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

				POINTER<VulkanSwapChain> swapChainPointer;
				POINTER<VulkanFrameBuffer> frameBufferPointer;

				ARRAY<VkBuffer> vertexBuffers;
				ARRAY<VkDeviceMemory> vertexBufferMemories;
				UI32 vertexCount = 0;

				ARRAY<VkBuffer> indexBuffers;
				ARRAY<VkDeviceMemory> indexBufferMemories;
				UI32 indexCount = 0;
				UI32 indexbufferObjectTypeSize = 0;

				ARRAY<ADGRVulkanTextureContainer> textures;

				ADGRVulkanDescrpitorContainer descriptors;

				ARRAY<VulkanPipeline> pipelineContainers;

				ARRAY<ADGRVulkanUnformBufferContainer> uniformBufferContainers;
				ARRAY<VulkanPushConstant> pushConstants;

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

				virtual void initializePipelineLayout(ADGRVulkanPipelineLayoutInitInfo info);
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
				void setFrameBufferContainer(POINTER<VulkanFrameBuffer> frameBuffer) { myRenderData.frameBufferPointer = frameBuffer; }
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
