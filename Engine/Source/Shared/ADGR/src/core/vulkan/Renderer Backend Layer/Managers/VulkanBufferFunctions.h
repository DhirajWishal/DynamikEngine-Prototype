#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
#define _DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H

#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanCoreObject.h"
#include "core/vulkan/Renderer Backend Layer/Managers/Objects/VulkanRenderObject.h"

#include "UniformBufferObject.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanCoreObject;
			class VulkanRenderObject;

			struct ADGRVulkanCommandBufferInitInfo {
				POINTER<ARRAY<VulkanRenderObject>> objects;
			};

			struct ADGRVulkanFrameBufferInitInfo {
				ARRAY<VkImageView> preAttachments;
				ARRAY<VkImageView> additionalAttachments;
			};

			struct ADGRVulkanVertexBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
			};

			struct ADGRVulkanIndexBufferContainer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 count = 0;
				UI32 typeSize = 0;
			};

			struct ADGRVulkanColorBufferContainer {
				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};

			struct ADGRVulkanDepthBufferContainer {
				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};

			class VulkanBufferFunctions {
				VulkanBufferFunctions() {}
				VulkanBufferFunctions(const VulkanBufferFunctions&) = delete;
				VulkanBufferFunctions operator=(const VulkanBufferFunctions&) = delete;
				VulkanBufferFunctions operator=(VulkanBufferFunctions&&) = delete;

			public:
				static void initializeCommandPool(POINTER<VulkanCoreObject> core);
				static void initializeCommandBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanCommandBufferInitInfo info);
				static void terminateCommandBuffer(POINTER<VulkanCoreObject> core);

				static void initializeFrameBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanFrameBufferInitInfo info);
				static void terminateFrameBuffer(POINTER<VulkanCoreObject> core);

				static void initializeVertexBuffer(POINTER<VulkanCoreObject> core, ARRAY<Vertex>* IndexBuffervertexes, POINTER<VulkanRenderObject> object);
				static void terminateVertexBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanVertexBufferContainer container);

				static void initializeIndexBufferUI8(POINTER<VulkanCoreObject> core, ARRAY<UI8>* indexes, POINTER<VulkanRenderObject> object);
				static void initializeIndexBufferUI16(POINTER<VulkanCoreObject> core, ARRAY<UI16>* indexes, POINTER<VulkanRenderObject> object);
				static void initializeIndexBufferUI32(POINTER<VulkanCoreObject> core, ARRAY<UI32>* indexes, POINTER<VulkanRenderObject> object);
				static void initializeIndexBufferUI64(POINTER<VulkanCoreObject> core, ARRAY<UI64>* indexes, POINTER<VulkanRenderObject> object);
				static void terminateIndexBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanIndexBufferContainer container);

				static void initializeUniformBuffer(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);
				static void updateUniformBuffer(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object, UniformBufferObject uniformBuferObject, UI32 currentImage);
				static void terminateUniformBuffer(POINTER<VulkanCoreObject> core, VulkanRenderObject object);

			public:
				static ADGRVulkanColorBufferContainer initializeColorBuffer(POINTER<VulkanCoreObject> core);
				static void terminateColorBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanColorBufferContainer container);

				static ADGRVulkanDepthBufferContainer initializeDepthBuffer(POINTER<VulkanCoreObject> core);
				static void terminateDepthBuffer(POINTER<VulkanCoreObject> core, ADGRVulkanDepthBufferContainer container);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_BUFFER_FUNCTIONS_H
