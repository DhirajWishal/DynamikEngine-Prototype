#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXTURE_FACTORY_H
#define _DYNAMIK_ADGR_VULKAN_TEXTURE_FACTORY_H

#include <vulkan/vulkan.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* Vulkan Texture base class */
			class VulkanTexture {
			public:
				VulkanTexture() {}
				virtual ~VulkanTexture() {}

				virtual void initialize() {}
				virtual void update() {}
				virtual void terminate() {}
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

#endif // !_DYNAMIK_ADGR_VULKAN_TEXTURE_FACTORY_H
