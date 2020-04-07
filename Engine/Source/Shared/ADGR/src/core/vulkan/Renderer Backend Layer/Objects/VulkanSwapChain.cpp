#include "adgrafx.h"
#include "VulkanSwapChain.h"

#include "VulkanFunctions.h"
#include "VulkanCore.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const ARRAY<VkSurfaceFormatKHR>& availableFormats) {
				for (const auto& availableFormat : availableFormats)
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
						&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
						return availableFormat;

				return ((ARRAY<VkSurfaceFormatKHR>)availableFormats)[0];
			}

			VkPresentModeKHR chooseSwapPresentMode(const ARRAY<VkPresentModeKHR>& availablePresentModes) {
				VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

				for (const auto& availablePresentMode : availablePresentModes) {
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return availablePresentMode;
					}
					else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
						bestMode = availablePresentMode;
				}

				return bestMode;
			}

			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, UI32 width, UI32 height) {
				if (capabilities.currentExtent.width != std::numeric_limits<UI32>::max())
					return capabilities.currentExtent;
				else {
					VkExtent2D actualExtent = {
						width,
						height
					};

					actualExtent.width = std::max(capabilities.minImageExtent.width,
						std::min(capabilities.maxImageExtent.width, actualExtent.width));
					actualExtent.height = std::max(capabilities.minImageExtent.height,
						std::min(capabilities.maxImageExtent.height, actualExtent.height));

					return actualExtent;
				}
			}

			void VulkanSwapChain::setBasicData(VkDevice device, VkPhysicalDevice physical, VkSurfaceKHR sur, VkSurfaceCapabilitiesKHR capabilities)
			{
				logicalDevice = device;
				physicalDevice = physical;
				surface = sur;
				surfaceCapabilities = capabilities;
			}

			void VulkanSwapChain::terminate()
			{
				terminateFrameBuffer();
				terminateRenderPass();
				terminateSwapChain();
			}

			void VulkanSwapChain::initializeSwapChain(UI32 width, UI32 height)
			{
				VulkanSwapChainSupportDetails swapChainSupport = querySwapChainSupport(&physicalDevice, &surface);

				VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

				VkCompositeAlphaFlagBitsKHR surfaceComposite =
					(surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
					? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
					: (surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
					: (surfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
					? VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
					: VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;

				UI32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0
					&& imageCount > swapChainSupport.capabilities.maxImageCount)
					imageCount = swapChainSupport.capabilities.maxImageCount;

				VkSwapchainCreateInfoKHR createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = surface;
				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				//createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

				ADGRVulkanQueue indices = VulkanCore::findQueueFamilies(physicalDevice, surface);
				UI32 queueFamilyindices[] = {
					indices.graphicsFamily.value(),
					indices.presentFamily.value()
				};

				if (indices.graphicsFamily != indices.presentFamily) {
					createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					createInfo.queueFamilyIndexCount = 2;
					createInfo.pQueueFamilyIndices = queueFamilyindices;
				}
				else {
					createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					createInfo.queueFamilyIndexCount = 0;
					createInfo.pQueueFamilyIndices = nullptr;
				}

				createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				createInfo.presentMode = presentMode;
				createInfo.clipped = VK_TRUE;
				createInfo.oldSwapchain = VK_NULL_HANDLE;

				if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain))
					DMK_CORE_FATAL("Failed to create Swap Chain!");

				vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
				swapChainImages.resize(imageCount);
				vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

				swapChainImageFormat = surfaceFormat.format;
				swapChainExtent = extent;

				initializeSwapChainImageViews();
			}

			void VulkanSwapChain::terminateSwapChain()
			{
				// destroy swapchain image views
				for (size_t i = 0; i < swapChainImageViews.size(); i++)
					vkDestroyImageView(logicalDevice, swapChainImageViews[i], nullptr);

				// destroy swapchain
				vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
			}

			void VulkanSwapChain::initializeSwapChainImageViews()
			{
				swapChainImageViews.resize(swapChainImages.size());

				for (UI32 i = 0; i < swapChainImages.size(); i++) {
					ADGRCreateImageViewInfo info;
					info.image = swapChainImages[i];
					info.format = swapChainImageFormat;
					info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
					info.mipLevels = 1;

					swapChainImageViews.at(i) = VulkanFunctions::createImageView(logicalDevice, info);
				}
			}

			void VulkanSwapChain::initializeRenderPass(ADGRVulkanRenderPassInitInfo info)
			{
				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = info.destinationSubpass;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = info.accessFlags;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				ARRAY<VkSubpassDependency> dependencies;
				dependencies.push_back(dependency);

				for (VkSubpassDependency _dependency : info.additionalSubPassDependencies)
					dependencies.push_back(_dependency);

				// render pass info
				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<UI32>(info.attachments.size());
				renderPassInfo.pAttachments = info.attachments.data();
				renderPassInfo.subpassCount = info.subPasses.size();
				renderPassInfo.pSubpasses = info.subPasses.data();
				renderPassInfo.dependencyCount = dependencies.size();
				renderPassInfo.pDependencies = dependencies.data();

				// create the render pass
				if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create render pass!");
			}

			void VulkanSwapChain::terminateRenderPass()
			{
				if (renderPass != VK_NULL_HANDLE)
					vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
			}

			void VulkanSwapChain::initializeFrameBuffer(ADGRVulkanFrameBufferInitInfo info)
			{
				frameBuffers.resize(swapChainImageViews.size());

				for (size_t i = 0; i < swapChainImageViews.size(); i++)
				{
					ARRAY<VkImageView> attachments;
					for (VkImageView _imageView : info.preAttachments)
						attachments.push_back(_imageView);

					attachments.push_back(swapChainImageViews[i]);

					for (VkImageView _imageView : info.additionalAttachments)
						attachments.push_back(_imageView);

					VkFramebufferCreateInfo framebufferInfo = {};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					framebufferInfo.attachmentCount = static_cast<UI32>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = swapChainExtent.width;
					framebufferInfo.height = swapChainExtent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS)
						DMK_CORE_FATAL("failed to create framebuffer!");
				}
			}

			void VulkanSwapChain::terminateFrameBuffer()
			{
				for (VkFramebuffer buffer : frameBuffers)
					vkDestroyFramebuffer(logicalDevice, buffer, nullptr);

				frameBuffers.clear();
			}

			void VulkanSwapChain::initializeDescriptorSetLayout(ADGRVulkanDescriptorSetLayoutInitInfo info)
			{
				VkDescriptorSetLayoutBinding uboLayoutBinding = {};
				uboLayoutBinding.binding = 0; // info.bindIndex;
				uboLayoutBinding.descriptorCount = 1;
				uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
				uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

				VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
				samplerLayoutBinding.binding = 1; // info.bindIndex;
				samplerLayoutBinding.descriptorCount = 1;
				samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
				samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				ARRAY<VkDescriptorSetLayoutBinding> bindings;
				bindings.push_back(uboLayoutBinding);
				bindings.push_back(samplerLayoutBinding);

				for (VkDescriptorSetLayoutBinding _binding : info.additionalBindings)
					bindings.push_back(_binding);

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = static_cast<UI32>(bindings.size());
				layoutInfo.pBindings = bindings.data();

				if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create descriptor set layout!");
			}

			void VulkanSwapChain::terminateDescriptorSetLayout()
			{
				vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
			}

			void VulkanSwapChain::initializePipelineLayout(ADGRVulkanPipelineLayoutInitInfo info)
			{
				ARRAY<VkDescriptorSetLayout> _layouts;
				_layouts.pushBack(descriptorSetLayout);

				for (auto _layout : info.additionalLayouts)
					_layouts.pushBack(_layout);

				// initialize the pipeline layout
				VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
				pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				pipelineLayoutInfo.setLayoutCount = _layouts.size();
				pipelineLayoutInfo.pSetLayouts = _layouts.data();

				if (info.pushConstantsEnable) {
					ARRAY<VkPushConstantRange> pushConstantInfos;
					pushConstantCount = info.pushConstantCount;
					pushConstants.resize(info.pushConstantCount);

					// initialize push constants
					for (I32 i = 0; i <= info.pushConstantCount; i++) {
						VkPushConstantRange pushConsInfo = {};
						pushConsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
						pushConsInfo.size = pushConstants.typeSize() * info.pushConstantCount;
						pushConsInfo.offset = info.pushConstantOffset;
						//pushConsInfo.offset = pushConstants.typeSize() * i;

						pushConstantInfos.push_back(pushConsInfo);
					}
					pipelineLayoutInfo.pushConstantRangeCount = info.pushConstantCount;	// make support for multiple
					if (info.pushConstantCount)
						pipelineLayoutInfo.pPushConstantRanges = pushConstantInfos.data();
					else
						pipelineLayoutInfo.pPushConstantRanges = nullptr;
				}

				// create the pipeline layout
				VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
				if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
					DMK_CORE_FATAL("failed to create pipeline layout!");

				pipelineLayout = _pipelineLayout;
			}

			void VulkanSwapChain::terminatePipelineLayout()
			{
				vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
			}

			VulkanSwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface)
			{
				VulkanSwapChainSupportDetails details;
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

				UI32 formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

				if (formatCount != 0) {
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
				}

				UI32 presentModeCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

				if (presentModeCount != 0) {
					details.presentModes.resize(presentModeCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface,
						&presentModeCount, details.presentModes.data());
				}

				return details;
			}
		}
	}
}