#include "adgrafx.h"
#include "VulkanReflectObject.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanReflectObject::initializeUniformBuffer()
			{
				VkDeviceSize bufferSize = sizeof(UBO_MPL);
				UI32 count = myRenderData.swapChainPointer->getSwapChainImages().size();

				myRenderData.uniformBuffers.resize(count);
				myRenderData.uniformBufferMemories.resize(count);

				for (size_t i = 0; i < count; i++)
				{
					ADGRCreateBufferInfo bufferInfo;
					bufferInfo.bufferSize = bufferSize;
					bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
					bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
					bufferInfo.buffer = &myRenderData.uniformBuffers[i];
					bufferInfo.bufferMemory = &myRenderData.uniformBufferMemories[i];

					VulkanFunctions::createBuffer(logicalDevice, physicalDevice, bufferInfo);
				}
			}
			
			void VulkanReflectObject::updateUniformBuffer(UBO_MPL uniformBufferObject, UI32 currentImage)
			{
				void* data = nullptr;
				vkMapMemory(logicalDevice, myRenderData.uniformBufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
				memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
				vkUnmapMemory(logicalDevice, myRenderData.uniformBufferMemories[currentImage]);
			}
			
			void VulkanReflectObject::initializeDescriptorSets(ADGRVulkanDescriptorSetsInitInfo info)
			{
				std::vector<VkDescriptorSetLayout> layouts(myRenderData.uniformBuffers.size(), myRenderData.swapChainPointer->getDescriptorSetLayout());
				myRenderData.descriptors.descriptorSets.resize(myRenderData.textures.size());

				for (UI32 itr = 0; itr < myRenderData.textures.size(); itr++) {
					for (size_t i = 0; i < myRenderData.uniformBuffers.size(); i++) {
						VkDescriptorSetLayout _layout = myRenderData.swapChainPointer->getDescriptorSetLayout();

						VkDescriptorSetAllocateInfo allocInfo = {};
						allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
						allocInfo.descriptorPool = myRenderData.descriptors.descriptorPools[i];
						allocInfo.descriptorSetCount = 1;
						allocInfo.pSetLayouts = &_layout;

						VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
						if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &_descriptorSet) != VK_SUCCESS)
							DMK_CORE_FATAL("failed to allocate descriptor sets!");

						VkDescriptorBufferInfo bufferInfo = {};
						bufferInfo.buffer = myRenderData.uniformBuffers[i];
						bufferInfo.offset = 0;
						bufferInfo.range = sizeof(UBO_MPL);

						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = myRenderData.textures[itr].imageView;
						imageInfo.sampler = myRenderData.textures[itr].imageSampler;

						ARRAY<VkWriteDescriptorSet> descriptorWrites = {};

						VkWriteDescriptorSet _writes1;
						_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes1.dstSet = _descriptorSet;
						_writes1.dstBinding = 0;
						_writes1.dstArrayElement = 0;
						_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						_writes1.descriptorCount = 1;
						_writes1.pBufferInfo = &bufferInfo;
						_writes1.pNext = VK_NULL_HANDLE;
						_writes1.pImageInfo = VK_NULL_HANDLE;
						_writes1.pTexelBufferView = VK_NULL_HANDLE;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstSet = _descriptorSet;
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;
						_writes2.pNext = VK_NULL_HANDLE;
						_writes2.pTexelBufferView = VK_NULL_HANDLE;

						descriptorWrites.push_back(_writes1);
						descriptorWrites.push_back(_writes2);

						for (VkWriteDescriptorSet _write : info.additionalWrites)
							descriptorWrites.push_back(_write);

						vkUpdateDescriptorSets(logicalDevice, static_cast<UI32>(descriptorWrites.size()),
							descriptorWrites.data(), 0, nullptr);

						myRenderData.descriptors.descriptorSets[itr].pushBack(_descriptorSet);
					} // make two descriptor layouts for each descriptor set
				}
			}
		}
	}
}