#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_DESCRIPTORS_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_DESCRIPTORS_MANAGER_H

#include "VulkanUniformBufferManager.h"
#include "Objects/VulkanCoreObject.h"
#include "Objects/VulkanRenderObject.h"

#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanRenderObject;
			struct ADGRVulkanUniformBufferContainer;

			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
			};

			struct ADGRVulkanDescriptorPoolInitInfo {
				ARRAY<VkDescriptorPoolSize> additionalSizes;
				UI32 poolCount = 3;
			};

			struct ADGRVulkanDescriptorSetsInitInfo {
				ARRAY<VkWriteDescriptorSet> additionalWrites;
			};

			struct ADGRVulkanDescrpitorContainer {
				VkDescriptorSetLayout layout;
				ARRAY<VkDescriptorPool> descriptorPools;
				ARRAY<ARRAY<VkDescriptorSet>> descriptorSets;
			};

			class VulkanDescriptorManager {
			public:
				VulkanDescriptorManager() {}
				~VulkanDescriptorManager() {}

				void initializeLayout(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetLayoutInitInfo info, POINTER<VulkanRenderObject> object);
				void terminateLayout(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				void initializeDescriptorPool(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorPoolInitInfo info, POINTER<VulkanRenderObject> object);
				void terminateDescriptorPool(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);

				void initializeDescriptorSets(POINTER<VulkanCoreObject> core, ADGRVulkanDescriptorSetsInitInfo info, POINTER<VulkanRenderObject> object);
				void terminateDescriptorSets(POINTER<VulkanCoreObject> core, POINTER<VulkanRenderObject> object);
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_DESCRIPTOR_SET_H
