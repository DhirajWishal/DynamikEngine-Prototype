#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H
#define _DYNAMIK_ADGR_VULKAN_SWAP_CHAIN_H

#include <vulkan/vulkan.h>
#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct VulkanSwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities = {};
				ARRAY<VkSurfaceFormatKHR> formats = {};
				ARRAY<VkPresentModeKHR> presentModes = {};
			};

			struct ADGRVulkanRenderPassInitInfo {
				UI32 destinationSubpass = 0;
				VkAccessFlags accessFlags = 0;
				B1 overrideDependencies = false;

				ARRAY<VkAttachmentDescription> attachments;

				ARRAY<VkSubpassDescription> subPasses;
				ARRAY<VkSubpassDependency> additionalSubPassDependencies;
			};

			struct ADGRVulkanFrameBufferInitInfo {
				ARRAY<VkImageView> preAttachments;
				ARRAY<VkImageView> additionalAttachments;
				VkRenderPass overrideRenderPass = VK_NULL_HANDLE;
				B1 useSwapChainImageView = true;
			};

			struct ADGRVulkanDescriptorSetLayoutInitInfo {
				ARRAY<VkDescriptorSetLayoutBinding> additionalBindings;
				B1 overrideBindings = false;
			};

			struct ADGRVulkanPipelineLayoutInitInfo {
				// push constants info
				B1 pushConstantsEnable = false;									// Vulkan push constants enable
				UI32 pushConstantCount = 1;										// Vulkan push constants count
				I32 pushConstantOffset = 0;										// Vulkan push constants offset

				ARRAY<VkDescriptorSetLayout> additionalLayouts;
			};

			class VulkanSwapChain {
			public:
				VulkanSwapChain() {}
				~VulkanSwapChain() {}

				void setBasicData(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities);
				void terminate();

				void initializeSwapChain(UI32 width, UI32 height);
				void terminateSwapChain();

				void initializeRenderPass(ADGRVulkanRenderPassInitInfo info);
				void terminateRenderPass();

				void initializeFrameBuffer(ADGRVulkanFrameBufferInitInfo info);
				void terminateFrameBuffer();

				virtual void initializeDescriptorSetLayout(ADGRVulkanDescriptorSetLayoutInitInfo into);
				virtual void terminateDescriptorSetLayout();

				virtual void initializePipelineLayout(ADGRVulkanPipelineLayoutInitInfo info);
				virtual void terminatePipelineLayout();

				const VkSwapchainKHR getSwapChain() const { return swapChain; }
				const ARRAY<VkImage> getSwapChainImages() const { return swapChainImages; }
				const ARRAY<VkImageView> getSwapChainImageViews() const { return swapChainImageViews; }
				const VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
				const VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }

				const ARRAY<VkFramebuffer> getFrameBuffers() const { return frameBuffers; }
				const VkFramebuffer getFrameBuffer(UI32 index) { return frameBuffers[index]; }

				const VkRenderPass getRenderPass() const { return renderPass; }

				const VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
				const VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
				const UI32 getPushConstantCount() const { return pushConstantCount; }

				static VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);

				ARRAY<glm::vec4> pushConstants;

			private:
				void initializeSwapChainImageViews();

				VkDevice logicalDevice = VK_NULL_HANDLE;
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
				VkSurfaceKHR surface = VK_NULL_HANDLE;
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};

				VkSwapchainKHR swapChain = VK_NULL_HANDLE;
				ARRAY<VkImage> swapChainImages;
				ARRAY<VkImageView> swapChainImageViews;
				VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
				VkExtent2D swapChainExtent = { 0, 0 };

				VkRenderPass renderPass = VK_NULL_HANDLE;

				ARRAY<VkFramebuffer> frameBuffers;

				VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
				UI32 pushConstantCount = 0;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_PIPELINE_H
