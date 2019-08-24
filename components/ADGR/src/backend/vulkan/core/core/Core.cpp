#include "adgrafx.h"
#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			GLFWwindow* Core::windowsWindow = nullptr;

			VkInstance Core::instance = VK_NULL_HANDLE;
			VkDevice Core::device = VK_NULL_HANDLE;
			VkPhysicalDevice Core::physicalDevice = VK_NULL_HANDLE;
			VkSurfaceKHR Core::surface = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT Core::debugMessenger = VK_NULL_HANDLE;
			VkSwapchainKHR Core::swapchain = VK_NULL_HANDLE;
			VkSurfaceCapabilitiesKHR Core::surfaceCapabilities;
			VkRenderPass Core::renderPass = VK_NULL_HANDLE;

			VkDescriptorSetLayout Core::descriptorSetLayout = VK_NULL_HANDLE;
			VkDescriptorPool Core::descriptorPool = VK_NULL_HANDLE;
			std::vector<VkDescriptorSet> Core::descriptorSets;

			VkCommandPool Core::commandPool = VK_NULL_HANDLE;
			std::vector<VkCommandBuffer> Core::commandBuffers;

			std::vector<VkSemaphore> Core::imageAvailableSemaphore;
			std::vector<VkSemaphore> Core::renderFinishedSemaphore;
			std::vector<VkFence> Core::inFlightFence;

			VkExtent2D Core::swapChainExtent;
			VkFormat Core::swapChainImageFormat = VK_FORMAT_UNDEFINED;
			std::vector<VkImage> Core::swapChainImages;
			std::vector<VkImageView> Core::swapChainImageViews;

			std::vector<VkFramebuffer> Core::frameBuffers;

			VkPipeline Core::graphicsPipeline = VK_NULL_HANDLE;
			VkPipelineLayout Core::pipelineLayout = VK_NULL_HANDLE;

			VkQueue Core::graphicsQueue = VK_NULL_HANDLE;
			VkQueue Core::presentQueue = VK_NULL_HANDLE;

			VkSampleCountFlagBits Core::msaaSamples = VK_SAMPLE_COUNT_1_BIT;

			VkImage Core::colorImage = VK_NULL_HANDLE;
			VkDeviceMemory Core::colorImageMemory = VK_NULL_HANDLE;
			VkImageView Core::colorImageView = VK_NULL_HANDLE;

			VkImage Core::depthImage = VK_NULL_HANDLE;
			VkDeviceMemory Core::depthImageMemory = VK_NULL_HANDLE;
			VkImageView Core::depthImageView = VK_NULL_HANDLE;

			VkImage Core::textureImage = VK_NULL_HANDLE;
			VkDeviceMemory Core::textureImageMemory = VK_NULL_HANDLE;
			VkImageView Core::textureImageView = VK_NULL_HANDLE;
			VkSampler Core::textureSampler = VK_NULL_HANDLE;

			uint32_t Core::mipLevels = 0;

			bool Core::frameBufferResizedEvent = false;
			unsigned int Core::WIDTH = 1280;
			unsigned int Core::HEIGHT = 720;

			float Core::minMipLevel;
			float Core::maxMipLevel;

			DMK_ShaderCode Core::vertexShaderCode;
			VkShaderModule Core::vertexShaderModule;
			VkPipelineShaderStageCreateInfo Core::vertexShaderStageInfo = {};

#ifdef USE_SHADER_TESSELLATION
			DMK_ShaderCode Core::tessellationShaderCode;
			VkShaderModule Core::tessellationShaderModule;
			VkPipelineShaderStageCreateInfo Core::tessellationShaderStageInfo = {};
#endif			

#ifdef USE_SHADER_GEOMETRY
			DMK_ShaderCode Core::geometryShaderCode;
			VkShaderModule Core::geometryShaderModule;
			VkPipelineShaderStageCreateInfo Core::geometryShaderStageInfo = {};
#endif
			DMK_ShaderCode Core::fragmentShaderCode;
			VkShaderModule Core::fragmentShaderModule;
			VkPipelineShaderStageCreateInfo Core::fragmentShaderStageInfo = {};
		}
	}
}
