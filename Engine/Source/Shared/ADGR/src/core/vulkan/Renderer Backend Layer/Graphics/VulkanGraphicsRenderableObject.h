#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDERABLE_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_GRAPHICS_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"

#include "VulkanGraphicsCore.h"
#include "VulkanGraphicsShader.h"
#include "VulkanGraphicsFrameBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanGraphicsPushConstant.h"
#include "VulkanGraphicsDescriptor.h"

#include "Attachments/VulkanGraphicsAttachment.h"
#include "Attachments/VulkanGraphicsTextureAttachment2D.h"
#include "Attachments/VulkanGraphicsUniformBufferAttachment.h"

#include <GameObject.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanGraphicsAttachment;
			struct ADGRVulkanTextureInitInfo;
			struct ADGRVulkanUnformBufferContainer;

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

			struct ADGRVulkanPiplineContainer {
				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout layout = VK_NULL_HANDLE;
			};

			struct ADGRVulkanRenderData {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC_OBJECT;
				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

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

				ARRAY<VulkanGraphicsAttachment> attachments;

				ARRAY<VulkanGraphicsPipeline> pipelineContainers;

				ARRAY<VulkanGraphicsPushConstant> pushConstants;

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

			class VulkanGraphicsRenderableObject : public GameObject {
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

				virtual void initializeTextures(ARRAY<ADGRVulkanTextureInitInfo> infos);
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

				virtual void initializeUniformBuffer();
				virtual void updateUniformBuffer(UniformBufferObject uniformBuferObject, UI32 currentImage);
				virtual void terminateUniformBuffer();

				void setSwapChainContainer(POINTER<VulkanGraphicsSwapChain> swapChain) { myRenderData.swapChainPointer = swapChain; }
				void setFrameBufferContainer(POINTER<VulkanGraphicsFrameBuffer> frameBuffer) { myRenderData.frameBufferPointer = frameBuffer; }
				void setRenderData(ADGRVulkanRenderData data) { myRenderData = data; }
				const ADGRVulkanRenderData getRenderData() const { return myRenderData; }

				ADGRVulkanRenderData myRenderData;
				ARRAY<ADGRVulkanUnformBufferContainer> uniformBuffers;

			protected:
				virtual void initializeNoTextureDescriptorSetLayout();
				virtual void initializeNoTexturePipelineLayout();

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;

				VkDescriptorSetLayout noTextureDescriptorSetLayout = VK_NULL_HANDLE;

				UI32 descriptorIndex = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERABLE_OBJECT_H
