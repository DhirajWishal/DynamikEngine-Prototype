#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_BUFFER_FACTORY_H
#define _DYNAMIK_RENDERER_VULKAN_BUFFER_FACTORY_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			/* Buffer initializer information structure */
			struct VulkanBufferInitInfo {
				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkCommandPool commandPool = VK_NULL_HANDLE;
				VkQueue processQueue = VK_NULL_HANDLE;
				VkQueue utilityQueue = VK_NULL_HANDLE;

				VPTR data = nullptr;
				UI32 byteSize = 0;
				UI32 typeSize = 0;
				UI32 dataCount = 0;
			};

			/* Buffer type enum */
			enum class VulkanBufferType {
				ADGR_VULKAN_BUFFER_TYPE_GENERAL,
				ADGR_VULKAN_BUFFER_TYPE_VERTEX,
				ADGR_VULKAN_BUFFER_TYPE_INDEX,
				ADGR_VULKAN_BUFFER_TYPE_UNIFORM,
			};

			/* Base class for all the Vulkan buffers */
			class VulkanBuffer {
			public:
				VulkanBuffer() {}
				VulkanBuffer(VulkanBufferType ty) : type(ty) {}
				virtual ~VulkanBuffer() {}

				virtual void initialize(VulkanBufferInitInfo info);
				virtual void update(VkDevice logicalDevice, VPTR data);
				virtual void terminate(VkDevice logicalDevice);

				const UI32 size() const { return byteSize; }

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
				UI32 dataCount = 0;

				VulkanBufferType type = VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_GENERAL;

			protected:
				UI32 byteSize = 0;
			};

			/* Vertex buffer */
			class VulkanVertexBuffer : public VulkanBuffer {
			public:
				VulkanVertexBuffer() : VulkanBuffer(VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_VERTEX) {}
				~VulkanVertexBuffer() {}

				void initialize(VulkanBufferInitInfo info) override;
			};

			/* Index buffer */
			class VulkanIndexBuffer : public VulkanBuffer {
			public:
				VulkanIndexBuffer() : VulkanBuffer(VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_INDEX) {}
				~VulkanIndexBuffer() {}

				void initialize(VulkanBufferInitInfo info) override;
			};

			/* Uniform buffer */
			class VulkanUniformBuffer : public VulkanBuffer {
			public:
				VulkanUniformBuffer() : VulkanBuffer(VulkanBufferType::ADGR_VULKAN_BUFFER_TYPE_UNIFORM) {}
				~VulkanUniformBuffer() {}

				void initialize(VulkanBufferInitInfo info) override;
			};

			/* Vulkan Buffer Factory */
			class VulkanBufferFactory {
				VulkanBufferFactory() {}
			public:
				~VulkanBufferFactory() {}

				static VulkanBuffer& createBuffer(VulkanBufferInitInfo info, VulkanBufferType type);
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_BUFFER_FACTORY_H
