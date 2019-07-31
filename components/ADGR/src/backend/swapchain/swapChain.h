#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

namespace Dynamik {
	namespace ADGR {
		namespace core {

			const int WIDTH = 800;
			const int HEIGHT = 600;

			struct swapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

			class swapChain {
			public:
				swapChain(VkDevice* device, VkSwapchainKHR* swapChain,
					VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface);
				~swapChain() {}

				void initSwapChain(GLFWwindow& window, std::vector<VkImage>* swapChainImages,
					VkFormat* swapChainImageFormat, VkExtent2D* swapChainExtent);
				void deleteSwapChain();

				void initImageViews(std::vector<VkImageView>* swapChainImageViews,
					std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat);
				void deleteImageViews(std::vector<VkImageView>* swapChainImageViews);

				void cleanUp(VkDevice device, std::vector<VkFramebuffer> swapChainFramebuffers, VkCommandPool commandPool,
					std::vector<VkCommandBuffer> commandBuffers, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout,
					VkRenderPass renderPass, std::vector<VkImageView> swapChainImageViews);

				swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
				VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
				VkExtent2D chooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities);

			private:
				VkDevice* myDevice;
				VkSwapchainKHR* mySwapChain;
				VkPhysicalDevice* myPhysicalDevice;
				VkSurfaceKHR* mySurface;

				VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
				VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			};
		}
	}
}
