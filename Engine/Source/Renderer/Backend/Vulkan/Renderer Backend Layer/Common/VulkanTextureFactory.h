#pragma once
#ifndef _DYNAMIK_RENDERER_VULKAN_TEXTURE_FACTORY_H
#define _DYNAMIK_RENDERER_VULKAN_TEXTURE_FACTORY_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			/* Vulkan Texture base class */
			class VulkanTexture {
			public:
				VulkanTexture() {}
				virtual ~VulkanTexture() {}

				virtual void initialize() {}
				virtual void update() {}
				virtual void terminate() {}

				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkSampler imageSampler = VK_NULL_HANDLE;

			protected:
				virtual void generateMipMap();
			};

			/* Texture 2D */
			class VulkanTexture2D : public VulkanTexture {
			public:
				VulkanTexture2D() {}
				~VulkanTexture2D() {}
			};

			/* Texture 3D */
			class VulkanTexture3D : public VulkanTexture {
			public:
				VulkanTexture3D() {}
				~VulkanTexture3D() {}
			};

			/* Cubemap texture */
			class VulkanTextureCubemap : public VulkanTexture {
			public:
				VulkanTextureCubemap() {}
				~VulkanTextureCubemap() {}
			};
		}
	}
}

#endif // !_DYNAMIK_RENDERER_VULKAN_TEXTURE_FACTORY_H
