#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		01/08/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "Renderer/backend/interface.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class SMP_API uniformBuffer {
			public:
				uniformBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					std::vector<VkBuffer>* uniformBuffer, std::vector<VkDeviceMemory>* uniformBuffersMemory,
					VkDescriptorPool* descriptorPool, std::vector<VkDescriptorSet>* descriptorSets,
					VkDescriptorSetLayout* descriptorSetLayout);
				~uniformBuffer();

				void createDescripterSetLayout();
				void initDescripterPool(std::vector<VkImage> swapChainImages);
				void initDescripterSets(std::vector<VkImage> swapChainImages,
					VkImageView textureImageView, VkSampler textureSampler);
				void deleteDescripter();

				void initBuffer(std::vector<VkImage> swapChainImages);
				void updateBuffer(uint32 currentImage, VkExtent2D swapChainExtent);

				VkDescriptorSetLayout* getDescriptorSetLayout() { return myDescriptorSetLayout; }

			private:
				VkDevice* myDevice;
				VkPhysicalDevice* myPhysicalDevice;
				VkDescriptorPool* myDescriptorPool;
				std::vector<VkDescriptorSet>* myDescriptorSets;
				std::vector<VkBuffer>* myUniformBuffer;
				std::vector<VkDeviceMemory>* myUniformBufferMemory;
				VkDescriptorSetLayout* myDescriptorSetLayout;
			};

			struct UniformBufferObject {
				alignas(16) glm::mat4 model;
				glm::mat4 view;
				glm::mat4 proj;
			};
		}
	}
}
