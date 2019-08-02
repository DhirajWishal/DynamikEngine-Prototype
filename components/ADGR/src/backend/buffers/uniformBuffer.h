#pragma once

#include "backend/interface.h"
#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API uniformBuffer {
			public:
				uniformBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice,
					std::vector<VkBuffer>* uniformBuffer, std::vector<VkDeviceMemory>* uniformBuffersMemory,
					VkDescriptorPool* descriptorPool, std::vector<VkDescriptorSet>* descriptorSets,
					VkDescriptorSetLayout* descriptorSetLayout);
				~uniformBuffer();

				void createDescripterSetLayout();
				void initDescripterPool(std::vector<VkImage> swapChainImages);
				void initDescripterSets(std::vector<VkImage> swapChainImages);
				void deleteDescripter();

				void initBuffer(std::vector<VkImage> swapChainImages);
				void updateBuffer(uint32 index, VkExtent2D swapChainExtent,
					uint32 currentImage);

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
				glm::mat4 model;
				glm::mat4 view;
				glm::mat4 proj;
			};
		}
	}
}
