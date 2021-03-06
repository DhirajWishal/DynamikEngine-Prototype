#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_GRAPHICS_CONTAINERS_H
#define _DYNAMIK_RENDERER_VULKAN_GRAPHICS_CONTAINERS_H

#include <vulkan/vulkan.h>
#include "Renderer/UniformBufferObject.h"

#include "VulkanGraphicsCore.h"
#include "VulkanGraphicsShader.h"
#include "VulkanGraphicsFrameBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanGraphicsPushConstant.h"
#include "VulkanGraphicsDescriptor.h"

#include "../Common/VulkanUtilities.h"

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			class VulkanGraphicsSwapChain;
			class VulkanGraphicsFrameBuffer;

			struct VulkanGraphicsRenderableObjectInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue graphicsQueue = VK_NULL_HANDLE;
				VkQueue presentQueue = VK_NULL_HANDLE;
			};

			struct VulkanGraphicsShaderPathContainer {
				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";
			};

			struct VulkanTextureInitInfo {
				UI32 mipLevels = 0;
				UI32 minMipLevels = 0;
				UI32 maxMipLevels = 0;

				VkFilter magFilter, minFilter;
				VkSamplerAddressMode modeU, modeV, modeW;
				VkImageAspectFlags aspectFlags;
			};

			struct VulkanMaterialDescriptor {
				struct PushBlock {
					F32 roughness;
					F32 metallic;
					F32 specular = 0.0f;
					F32 r, g, b;
				} params;

				std::string name;

				VulkanMaterialDescriptor() {}
				VulkanMaterialDescriptor(std::string n, glm::vec3 c, F32 r, F32 m) : name(n) {
					params.roughness = r;
					params.metallic = m;
					params.r = c.r;
					params.g = c.g;
					params.b = c.b;
				}
				~VulkanMaterialDescriptor() {}
			};

			struct VulkanPiplineContainer {
				VkPipeline pipeline = VK_NULL_HANDLE;
				VkPipelineLayout layout = VK_NULL_HANDLE;
			};

			struct VulkanBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 dataCount = 0;
			};

			struct VulkanRenderObject {
				DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;

				VulkanGraphicsPipeline pipeline;

				std::vector<VulkanBufferContainer> vertexBufferContainer;

				std::vector<VulkanBufferContainer> indexBufferContainer;
				DMKDataType indexBufferType = DMKDataType::DMK_DATA_TYPE_UI32;

				VulkanGraphicsDescriptor descriptors;
				std::vector<VulkanUnformBufferContainer> uniformBufferContainers;

				std::vector<VulkanGraphicsPushConstant> pushConstants;

				MeshVertexLimits limits;
			};

			struct VulkanRenderData {
				DMKRenderingTechnology renderTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;

				POINTER<VulkanGraphicsSwapChain> swapChainPointer;
				POINTER<VulkanGraphicsFrameBuffer> frameBufferPointer;

				std::vector<VulkanRenderObject> renderObject;

				std::vector<VulkanTextureContainer> textures;

				VulkanMaterialDescriptor materialDescriptor;
				B1 enableMaterials = false;
			};

			struct StaggingBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct VulkanGraphicsCommandBufferInitResources {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
			};

			struct VulkanSurfaceContainer {
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_GRAPHICS_CONTAINERS_H
