#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDERABLE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include "Engines/ADGR/UniformBufferObject.h"

#include "VulkanGraphicsCore.h"
#include "VulkanGraphicsShader.h"
#include "VulkanGraphicsFrameBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanGraphicsPushConstant.h"
#include "VulkanGraphicsDescriptor.h"

#include "Objects/InternalFormat/InteralFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanGraphicsRenderableObjectInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};

			struct ADGRVulkanGraphicsShaderPathContainer {
				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";
			};

			struct ADGRVulkanTextureInitInfo {
				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			struct ADGRVulkanMaterialDescriptor {
				struct PushBlock {
					F32 roughness;
					F32 metallic;
					F32 specular = 0.0f;
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

			struct ADGRVulkanUnformBufferContainer {
				ARRAY<VkBuffer> buffers;
				ARRAY<VkDeviceMemory> bufferMemories;
			};

			struct ADGRVulkanPiplineContainer {
				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout layout = VK_NULL_HANDLE;
			};

			struct ADGRVulkanRenderData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;
				DMKRenderingTechnology renderTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;

				POINTER<VulkanGraphicsSwapChain> swapChainPointer;
				POINTER<VulkanGraphicsFrameBuffer> frameBufferPointer;

				ARRAY<VkBuffer> vertexBuffers;
				ARRAY<VkDeviceMemory> vertexBufferMemories;
				UI32 vertexCount = 0;

				ARRAY<VkBuffer> indexBuffers;
				ARRAY<VkDeviceMemory> indexBufferMemories;
				UI32 indexCount = 0;
				UI32 indexbufferObjectTypeSize = 0;

				ARRAY<ADGRVulkanTextureContainer> textures;

				VulkanGraphicsDescriptor descriptors;

				ARRAY<VulkanGraphicsPipeline> pipelineContainers;

				ARRAY<ADGRVulkanUnformBufferContainer> uniformBufferContainers;
				ARRAY<VulkanGraphicsPushConstant> pushConstants;

				ADGRVulkanMaterialDescriptor materialDescriptor;
				B1 enableMaterials = false;
			};

			struct StaggingBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct ADGRVulkan3DObjectData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;
				std::string modelpath = "";
				std::string materialName = "Metal";
				B1 isPBR = false;

				POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				ARRAY<std::string> texturePaths;

				DMKRenderingTechnology renderTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;
			};

			class VulkanGraphicsRenderableObject : public InternalFormat {
			public:
				VulkanGraphicsRenderableObject() {}
				VulkanGraphicsRenderableObject(ADGRVulkanGraphicsRenderableObjectInitInfo info)
					: logicalDevice(info.logicalDevice), physicalDevice(info.physicalDevice), commandPool(info.commandPool), graphicsQueue(info.graphicsQueue), presentQueue(info.presentQueue) {}
				virtual ~VulkanGraphicsRenderableObject() {}

				virtual void initializeResources(ADGRVulkanGraphicsRenderableObjectInitInfo info);

				virtual ADGRVulkanRenderData initializeObject(VkDevice logicalDevice, ADGRVulkan3DObjectData _object, VkSampleCountFlagBits msaaSamples);

				virtual void initializePipelineLayout(ADGRVulkanGraphicsPipelineLayoutInitInfo info);
				virtual void initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info);
				virtual void terminatePipeline();

				virtual void initializeTextures(ADGRVulkanTextureInitInfo info);
				virtual void generateMipMaps(POINTER<ADGRVulkanTextureContainer> container);
				virtual void terminateTextures();

				virtual void initializeVertexBuffer();
				virtual void terminateVertexBuffer();

				virtual void initializeIndexBuffer();
				virtual void terminateIndexBuffer();

				virtual void initializeDescriptorPool();
				virtual void terminateDescriptorPool();

				virtual void initializeDescriptorSets();

				virtual void initializeUniformBuffer();
				virtual void updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage);
				virtual void terminateUniformBuffer();

				void setSwapChainContainer(POINTER<VulkanGraphicsSwapChain> swapChain) { myRenderData.swapChainPointer = swapChain; }
				void setFrameBufferContainer(POINTER<VulkanGraphicsFrameBuffer> frameBuffer) { myRenderData.frameBufferPointer = frameBuffer; }
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
