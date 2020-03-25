#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DESCRIPTORS_H
#define _DYNAMIK_ADGR_VULKAN_DESCRIPTORS_H

#include "VulkanSwapChain.h"
#include "VulkanUniformBuffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanTexture;

			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				VulkanDevice device;

				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
			};

			struct ADGRVulkanDescriptorPoolInitInfo {
				VulkanDevice device;
				VulkanSwapChain swapChain;

				ARRAY<VkDescriptorPoolSize> additionalSizes;
				UI32 poolCount = 3;
			};

			struct ADGRVulkanDescriptorSetsInitInfo {
				VulkanDevice device;
				VulkanSwapChain swapChain;
				VulkanUniformBuffer uniformBuffers;
				ARRAY<VulkanTexture> textures;

				ARRAY<VkWriteDescriptorSet> additionalWrites;
			};

			class VulkanDescriptors {
			public:
				VulkanDescriptors() {}
				~VulkanDescriptors() {}

				void initializeLayout(ADGRVulkanDescriptorSetLayoutInitInfo info);
				void terminateLayout(VulkanDevice device);

				void initializeDescriptorPool(ADGRVulkanDescriptorPoolInitInfo info);
				void terminateDescriptorPool(VulkanDevice device);

				void initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info);
				void terminateDescriptorSets(VulkanDevice device);

				VkDescriptorSetLayout layout;
				ARRAY<VkDescriptorPool> descriptorPools;
				ARRAY<ARRAY<VkDescriptorSet>> descriptorSets;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DESCRIPTOR_SET_H
