#pragma once

/*
 Vulkan specific data containers and enums for the Dynamik Engine.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 vulkan.h file
*/

#ifndef _DYNAMIK_ADGR_VULKAN_H
#define _DYNAMIK_ADGR_VULKAN_H
#if defined(DMK_USE_VULKAN)

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			/* SHADER STAGE INFO ENUM */
			enum DMK_ADGR_VULKAN_SHADER_STAGE_INFO {
				DMK_ADGR_VULKAN_SHADER_STAGE_VERTEX = 0,							// vertex shader
				DMK_ADGR_VULKAN_SHADER_STAGE_TESSELLATION = 1,						// tessellation shader
				DMK_ADGR_VULKAN_SHADER_STAGE_GEOMETRY = 2,							// geometry shader
				DMK_ADGR_VULKAN_SHADER_STAGE_FRAGMENT = 3							// fragment shader
			};

			/* SWAPCHAIN DATA CONTAINER */
			struct ADGRVulkanSwapchainDataContainer {
				VkFormat swapchainImageFormat = VkFormat::VK_FORMAT_UNDEFINED;		// Vulkan swapchain image format
				VkExtent2D swapchainExtent = {										// Vulkan swapchain extent 2D
					VK_NULL_HANDLE,	// width
					VK_NULL_HANDLE	// height
				};
				std::vector<VkImageView> swapchainImageViews = {};					// Vulkan swapchain image views
				std::vector<VkImage> swapChainImages = {};							// Vulkan swapchain images
				VkSwapchainKHR swapchain = VK_NULL_HANDLE;							// Vulkan swapchain
			};

			/* PIPELINE DATA CONTAINER */
			struct ADGRVulkanPipelineDataContainer {
				std::vector<DMK_ShaderCode> shaderCodes = {							// Vulkan shader codes
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
				std::vector<VkShaderModule> shaderModules = {						// Vulkan shader modules
					VK_NULL_HANDLE,	// vertex shader
					VK_NULL_HANDLE,	// tessellation shader
					VK_NULL_HANDLE,	// geometry shader
					VK_NULL_HANDLE	// fragment shader
				};
				std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo = {	// Vulkan shader stage create info
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
				VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;					// Vulkan pipeline layout
				VkPipeline pipeline = VK_NULL_HANDLE;								// Vulkan pipeline
			};

			// Shader data
			struct ADGRVulkanShaderDataContainer {
				std::vector<DMK_ShaderCode> shaderCodes = {							// Vulkan shader codes
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
				std::vector<VkShaderModule> shaderModules = {						// Vulkan shader modules
					VK_NULL_HANDLE,	// vertex shader
					VK_NULL_HANDLE,	// tessellation shader
					VK_NULL_HANDLE,	// geometry shader
					VK_NULL_HANDLE	// fragment shader
				};
				std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfo = {	// Vulkan shader stage create info
					{},	// vertex shader
					{},	// tessellation shader
					{},	// geometry shader
					{}	// fragment shader
				};
			};

			// Pipeline creation info
			struct DMKPipelineInitInfo {
				ADGRVulkanShaderDataContainer shaderDataContainer = {};				// Vulkan shader data container
				VkRenderPass renderPass = VK_NULL_HANDLE;							// Vulkan render pass

				std::vector<VkDescriptorSetLayout_T*> layouts = {};					// Vulkan descriptor set layouts

				// primitive assembly info
				VkPrimitiveTopology inputAssemblyTopology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;	// Vulkan input assembler topologies
				VkBool32 inputAssemblyPrimitiveRestartEnable = VK_FALSE;			// Vulkan primitive assembly restart enable

				// viewport info
				UI32 viweportCount = 1;											// Vulkan view port count

				// scissor info
				UI32 scissorCount = 1;											// Vulkan scissor count
				std::vector<VkOffset2D> offsets = { { 0, 0 } };						// Vulkan scissor offsets

				// rasterizer info
				VkBool32 rasterizerDepthClampEnable = VK_FALSE;						// Vulkan rasterizer depth clamp enable
				VkBool32 rasterizerDiscardEnable = VK_FALSE;						// Vulkan rasterizer discard enable
				VkPolygonMode rasterizerPolygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;			// Vulkan rasterizer polygon mode
				F32 rasterizerLineWidth = 1.0f;									// Vulkan rasterizer line width
				VkCullModeFlagBits rasterizerCullMode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;		// Vulkan rasterizer cull mode
				VkFrontFace rasterizerFrontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;	// Vulkan rasterizer front face
				VkBool32 rasterizerDepthBiasEnable = VK_FALSE;						// Vulkan rasterizer depth bias enable

				// multisampling info
				VkSampleCountFlagBits multisamplerMsaaSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;	// Vulkan multisampler MSAA samples
				VkBool32 multisamplerSampleShadingEnable = VK_TRUE;					// Vulkan multisampler sample shading enable
				F32 multisamplerMinSampleShading = 0.2f;							// Vulkan multisampler sample shading

				// depth stencil info
				VkBool32 depthStencilEnable = VK_TRUE;								// Vulkan depth stencil enable
				VkBool32 depthStencilWriteEnable = VK_TRUE;							// Vulkan depth stencil write enable
				VkCompareOp depthStencilCompareOp = VkCompareOp::VK_COMPARE_OP_LESS;	// Vulkan depth stencil compare op
				VkBool32 depthStencilBoundsTestEnable = VK_FALSE;					// Vulkan depth stencil bounds test enable
				VkBool32 depthStencilTestEnable = VK_FALSE;							// Vulkan depth stencil test enable

				// color blender info
				std::vector<VkColorComponentFlags> colorBlenderColorWriteMasks = {	// Vulkan color blend write masks
					VK_COLOR_COMPONENT_R_BIT	// Red
					| VK_COLOR_COMPONENT_G_BIT	// Green
					| VK_COLOR_COMPONENT_B_BIT	// Blue
					| VK_COLOR_COMPONENT_A_BIT	// Alpha
				};
				std::vector<VkBool32> colorBlenderBlendEnables = { VK_FALSE };		// Vulkan color blend blend enables

				VkBool32 colorBlendingLogicOpEnable = VK_FALSE;						// Vulkan color blend blending logical op enable
				VkLogicOp colorBlendingLogicOp = VkLogicOp::VK_LOGIC_OP_COPY;		// Vulkan color blend blending logical op
				UI32 colorBlendingColorBlendCount = 1;							// Vulkan color blend blending count
				std::vector<F32> colorBlendingBlendConstants = {					// Vulkan color blend constants
					0.0f,	// Red
					0.0f,	// Green
					0.0f,	// Blue
					0.0f	// Alpha
				};

				// push constants info
				B1 pushConstantsEnable = false;									// Vulkan push constants enable
				UI32 pushConstantCount = 1;										// Vulkan push constants count
				I32 pushConstantOffset = 0;										// Vulkan push constants offset

				// dynamic state info
				B1 dynamicStateEnable = false;									// Vulkan dynamic state enable
				VkPipelineDynamicStateCreateFlags dynamicStateFlags = 0;			// Vulkan dynamic state flags

				// pipeline info
				UI32 pipelineStageCount = 2;									// Vulkan pipeline stage count
				UI32 pipelineSubPass = 0;										// Vulkan pipeline sub pass
				VkPipeline pipelineBasePipelineHandle = VK_NULL_HANDLE;				// Vulkan base pipeline handle
				UI32 pipelineBasePipelineIndex = 0;
			};

			/* COMMAND BUFFER CONTAINER */
			struct ADGRVulkanCommandBufferDataContainer {
				VkCommandPool commandPool = VK_NULL_HANDLE;							// Vulkan command pool
				std::vector<VkCommandBuffer> buffers = {};							// Vulkan command buffers
			};

			/* COLOR BUFFER CONTAINER */
			struct ADGRVulkanColorBufferDataContainer {
				VkImageView imageView = VK_NULL_HANDLE;								// Vulkan color image view
				VkImage image = VK_NULL_HANDLE;										// Vulkan color image
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;						// Vulkan color image memory
			};

			/* DEPTH BUFFER CONTAINER */
			struct ADGRVulkanDepthBufferDataContainer {
				VkImageView imageView = VK_NULL_HANDLE;								// Vulkan depth image view
				VkImage image = VK_NULL_HANDLE;										// Vulkan depth image
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;						// Vulkan depth image memory
			};

			/* FRAME BUFFER CONTAINER */
			struct ADGRVulkanFrameBufferDataContainer {
				std::vector<VkFramebuffer> buffers = {};							// Vulkan frame buffers
			};

			/* DESCRIPTOR SETS CONATAINER */
			struct ADGRVulkanDescriptorSetsDataContainer {
				VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;			// Vulkan descriptor set layout
				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;					// Vulkan descriptor pool
				std::vector<VkDescriptorSet> descriptorSets = {};					// Vulkan descriptor sets
			};

			/* SEMAPHORES AND FENCES CONTAINER */
			struct ADGRVulkanSemaphoresAndFencesDataContainer {
				std::vector<VkSemaphore> imageAvailableSemaphore = {};				// Vulkan image available semaphores
				std::vector<VkSemaphore> renderFinishedSemaphore = {};				// Vulkan render finished semaphores
				std::vector<VkFence> inFlightFence = {};							// Vulkan in-flight fences
			};

			/* DATA CONTAINER */
			struct ADGRVulkanDataContainer {
				/* window */
				GLFWwindow* window = nullptr;										// GLFW window

				/* VULKAN CORE */
				VkInstance instance = VK_NULL_HANDLE;								// Vulkan instance
				VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;			// Vulkan debug messenger
				VkSurfaceKHR surface = VK_NULL_HANDLE;								// Vulkan GLFW window surface
				VkSurfaceCapabilitiesKHR surfaceCapabilities = {};					// Vulkan GLFW window surfce capabilities
				VkDevice device = VK_NULL_HANDLE;									// Vulkan device
				VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;					// Vulkan physical device

				/* SWAPCHAIN */
				ADGRVulkanSwapchainDataContainer swapchainContainer = {};			// ADGR swapchain container

				/* PIPELINE */
				std::vector<ADGRVulkanPipelineDataContainer> pipelineContainers = {};	// ADGR pipeline containers
				VkRenderPass renderPass = VK_NULL_HANDLE;							// Vulkan render pass

				/* COMMAND BUFFER */
				ADGRVulkanCommandBufferDataContainer commandBufferContainer = {};	// ADGR command buffer container

				/* COLOR BUFFER */
				ADGRVulkanColorBufferDataContainer colorBufferContainer = {};		// ADGR color buffer container

				/* DEPTH BUFFER */
				ADGRVulkanDepthBufferDataContainer depthBufferContainer = {};		// ADGR depth buffer container

				/* FRAME BUFFER */
				ADGRVulkanFrameBufferDataContainer frameBufferContainer = {};		// ADGR frame buffer container

				/* DESCRIPTOR SETS */
				std::vector<ADGRVulkanDescriptorSetsDataContainer> descriptorSetsContainers = {};	// ADGR descriptor set container

				/* SEMAPHORES AND FENCES */
				ADGRVulkanSemaphoresAndFencesDataContainer semaphoresAndFencesContainer = {};	// ADGR semaphores and fences container

				/* BLANK SCREEN BACKGROUND COLOR */
				std::vector<F32> clearScreenValues = {							// Vulkan clear screen values
					(2.0f / 256.0f),		// Red
					(8.0f / 256.0f),		// Green
					(32.0f / 256.0f),		// Blue
					(1.00000000f)			// Alpha
				};	// Dynwamik color code: rgba(2, 8, 32, 1)

				/* QUEUES */
				VkQueue graphicsQueue = VK_NULL_HANDLE;								// Vulkan graphics queue
				VkQueue presentQueue = VK_NULL_HANDLE;								// Vulkan present queue

				/* MSAA BITS */
				VkSampleCountFlagBits msaaSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;	// Vulkan msaa sample bits

				/* CORE EVENTS */
				B1 frameBufferResizedEvent = false;								// Vulkan GLFW frame buffer resized event
				UI32 WIDTH = 720;													// Vulkan GLFW window width
				UI32 HEIGHT = 1280;													// Vulkan GLFW window height

				/* CORE MIP LEVELS */
				F32 minMipLevel = 0.0f;											// Vulkan minimum mipmap level
				F32 maxMipLevel = 0.0f;											// Vulkan maximum mipmap level

				std::array<glm::vec4, 6> pushConstants;								// Vulkan push constants
			};

			struct DMKObjectData {
				std::string path = "";
				std::vector<F32> offsets = { 0.0f, 0.0f, 0.0f };

				std::vector<Vertex>* vertexBufferObject = {};
				std::vector<VkBuffer>* vertexBuffers = {};
				std::vector<VkDeviceMemory>* vertexBufferMemories = {};

				std::vector<UI32>* indexBufferObject = {};
				VkBuffer* indexBuffer = VK_NULL_HANDLE;
				VkDeviceMemory* indexBufferMemory = VK_NULL_HANDLE;

				UI32* vertexCount = 0;
				UI32* indexCount = 0;
			};

			enum DMKVulkanRendererCreateBufferType {
				VERTEX_BUFFER,
				INDEX_BUFFER
			};

			struct DMKVulkanRendererLoadObjectInfo {
				DMKObjectData objectDataContainer = {};
			};

			struct DMKVulkanRendererCreateVertexBufferInfo {
				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* bufferMemory = VK_NULL_HANDLE;

				std::vector<Vertex>* vertexBufferObject = {};
			};

			struct DMKVulkanRendererCreateIndexBufferInfo {
				VkBuffer* buffer = VK_NULL_HANDLE;
				VkDeviceMemory* bufferMemory = VK_NULL_HANDLE;

				std::vector<UI32>* indexBufferObject = {};
			};

			struct DMKVulkanRendererCreateUniformBufferInfo {
				std::vector<VkBuffer>* buffer = {};
				std::vector<VkDeviceMemory>* bufferMemory = {};
			};

			struct DMKVulkanRendereCreateDescriptorSetsInfo {
				std::vector<VkBuffer>* uniformBuffers = {};
				VkImageView textureImageView = VK_NULL_HANDLE;
				VkSampler textureSampler = VK_NULL_HANDLE;

				std::vector<VkDescriptorSet>* descriptorSets = {};
				VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
				VkDescriptorSetLayout* layout = VK_NULL_HANDLE;
				std::vector<uint8_t> bindIndexes = { 0, 1 };
			};

			struct DMKVulkanBuffer {
				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanTexture {
				VkImage texture = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkSampler sampler = VK_NULL_HANDLE;

				UI32 mipLevels = 1;
			};

			struct DMKVulkanVertexBuffer {
				std::vector<Vertex> vertexBuffers = {};

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanIndexBuffer {
				std::vector<UI32> indexBuffers = {};

				VkBuffer buffer = VK_NULL_HANDLE;
				VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
			};

			struct DMKVulkanSkyboxCommandBufferBindData {
				VkBuffer vertexBuffer = VK_NULL_HANDLE;
				VkBuffer indexBuffer = VK_NULL_HANDLE;
				std::vector<UI32> indexes = {};

				VkPipeline pipeline = VK_NULL_HANDLE;
				VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
			};

			struct DMKVulkanSkyboxContainer {
				std::string path = "";

				VkImage skyboxImage = VK_NULL_HANDLE;
				VkImageLayout skyboxLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				VkSampler skyboxSampler = VK_NULL_HANDLE;

				VkFormat skyboxFormat = VK_FORMAT_UNDEFINED;
				VkDeviceMemory skyboxMemory = VK_NULL_HANDLE;

				UI32 mipLevels = NULL;

				DMKVulkanSkyboxCommandBufferBindData commandBufferBindData = {};

				VkPipeline pipeline = VK_NULL_HANDLE;
			};
		}
	}
}

#endif	
#endif	// !_DYNAMIK_ADGR_VULKAN_H
