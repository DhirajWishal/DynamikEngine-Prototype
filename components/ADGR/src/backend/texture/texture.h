#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		02/08/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "core/utils/DMK_DataTypes.h"
#include "backend/interface.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width,
				uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
				VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory,
				uint32 mipLevels, VkSampleCountFlagBits numSamples);
			void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkImage image,
				VkFormat format, VkImageLayout oldLayout,
				VkImageLayout newLayout, VkQueue graphicsQueue, uint32 mipLevels);
			VkImageView createImageView(VkDevice device, VkImage image, VkFormat format,
				VkImageAspectFlags aspectFlags, uint32 mipLevels);

			class ADGR_API texture {
			public:
				texture(VkDevice* device, VkPhysicalDevice* physicalDevice,
					VkBuffer* stagingBuffer, VkDeviceMemory* stagingBufferMemory,
					VkImage* txImage, VkDeviceMemory* txImageMemory,
					VkImageView* textureImageView, VkSampler* textureSampler,
					uint32* mipLevels);
				~texture();

				void initTexture(VkCommandPool commandPool, VkQueue graphicsQueue);
				void initTextureImageView();
				void initTextureSampler();
				void deleteTexture();

				void generateMipMaps(VkFormat imageFormat, VkCommandPool commandPool, VkQueue graphicsQueue);

				float minMipLevel = 0.0f;
			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkBuffer* myStagingBuffer;
				VkDeviceMemory* myStagingBufferMemory;
				VkImage* myTextureImage;
				VkDeviceMemory* myTextureImageMemory;
				VkImageView* myTextureImageView;
				VkSampler* myTextureSampler;

				uint32* myMipLevels;

				int texWidth, texHeight, texChannels;
			};
		}
	}
}
