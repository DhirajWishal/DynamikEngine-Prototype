#pragma once

/*
 Manager for the Advanced Dynamik Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		02/12/2019
 IDE:		MS Visual Studio Community 2019

 manager.h file
*/

#ifndef _DYNAMIK_ADGR_VULKAN_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_MANAGER_H
#ifdef DMK_USE_VULKAN

#include "CentralDataHub.h"

#include "GLFW/glfw3.h"
#include "data structures/vulkan.h"
#include "GameObject.h"
#include "commandBufferManager.h"
#include "vulkanFormat.h"

#include "core/log.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			/*
				Use addresses as much as possible for more memory efficiency.
			*/
			class manager : public Manager {
			public:
				// Default constructor
				manager() {
					shaderModules.resize(4);
				}

				// Default destructor
				// Clear all locally stored variables
				~manager() {}

				// Add a resource to the manager
				template<typename _Type>
				void addResource(_Type& data, DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					((std::vector<_Type>*)_resource(type))->push_back(data);
				}

				// Set a resource to the manager
				template<typename _Type>
				void setResource(_Type& data, DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					*((_Type*)_resource(type)) = data;
				}

				// Clear resource store from the manager
				template<typename _Type>
				void clearResource(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					(std::vector<_Type>*)_resource(type)->clear();
				}

				// Clear a sub resource from the manager
				template<typename _Type>
				void clearResource(I32 index, DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					(std::vector<_Type>*)_resource(type)->at(index).clear();
				}

				// Allocate a size to a perticular store
				template<typename _Type>
				void allocate(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN, I32 size = 1) {
					((std::vector<_Type>*)_resource(type))->resize(size);
				}

				// Get full resource from the manager
				template<typename _Type>
				std::vector<_Type>& getFullResource(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					return *((std::vector<_Type>*)_resource(type));
				}

				// Get full resource address from the manager
				template<typename _Type>
				std::vector<_Type>* getFullResourceAddr(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) {
					return ((std::vector<_Type>*)_resource(type));
				}

				// Get a resource from the manager
				template<typename _Type>
				_Type& getResource(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN, I32 index = 0) {
					I32 _resourceSize = ((std::vector<_Type>*)_resource(type))->size();

					if (index > _resourceSize)
						DMK_CORE_FATAL("Invalid resource index!");

					return ((std::vector<_Type>*)_resource(type))->at(index);
				}

				// Get a resource address from the manager
				template<typename _Type>
				_Type* getResourceAddr(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN, I32 index = 0) {
					I32 _resourceSize = ((std::vector<_Type>*)_resource(type))->size();

					if (index > _resourceSize)
						DMK_CORE_FATAL("Invalid resource index!");

					return &((std::vector<_Type>*)_resource(type))->at(index);
				}

				// Get resource size (array size)
				template<typename _Type>
				I32 getResourceSize(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) const {
					return ((std::vector<_Type>*)_resource(type))->size();
				}

				// Get resource size (array size) with an index
				template<typename _Type>
				I32 getResourceSize(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN, I32 index = 0) {
					return ((std::vector<_Type>*)_resource(type))->at(index).size();
				}

			private:
				// Get the specified resource address
				const void* _resource(DMKManagerResourceType type = DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN) override {
					const void* _returnData = nullptr;

					switch (type) {
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER:
						_returnData = &vulkanDataContainers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INSTANCE:
						_returnData = &instances;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEVICE:
						_returnData = &devices;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PHYSICAL_DEVICE:
						_returnData = &physicalDevices;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_WINDOW:
						_returnData = &windows;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SURFACE:
						_returnData = &surfaces;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEBUGGER:
						_returnData = &debugMessengers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN:
						_returnData = &swapchains;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGES:
						_returnData = &swapchainImages;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGE_FORMAT:
						_returnData = &swapchainImageFormats;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_EXTENT:
						_returnData = &swapchainImageExtent;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGE_VIEWS:
						_returnData = &swapchainImageViews;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH:
						_returnData = &shaderPaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_VERTEX:
						_returnData = &shaderModules[0];
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_TESSELLATION:
						_returnData = &shaderModules[1];
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_GEOMETRY:
						_returnData = &shaderModules[2];
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_FRAGMENT:
						_returnData = &shaderModules[3];
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE_RENDER_PASS:
						_returnData = &pipelineRenderPasses;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE:
						_returnData = &pipelines;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE_LAYOUT:
						_returnData = &pipelineLayouts;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER:
						_returnData = &colorBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE:
						_returnData = &colorBufferImages;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE_MEMORY:
						_returnData = &colorBufferImageMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE_VIEW:
						_returnData = &colorBufferImageViews;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER:
						_returnData = &depthBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE:
						_returnData = &depthBufferImages;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE_MEMORY:
						_returnData = &depthBufferImageMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE_VIEW:
						_returnData = &depthBufferImageViews;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FRAME_BUFFER:
						_returnData = &frameBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE:
						_returnData = &textureImages;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_MEMORY:
						_returnData = &textureImageMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_SAMPLER:
						_returnData = &textureImageSamplers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_VIEW:
						_returnData = &textureImageViews;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER:
						_returnData = &vertexBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER_OBJECT:
						_returnData = &vertexBufferObjects;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER_MEMORY:
						_returnData = &vertexBufferMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER:
						_returnData = &indexBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER_OBJECT:
						_returnData = &indexBufferObjects;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER_MEMORY:
						_returnData = &indexBufferMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER:
						_returnData = &uniformBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES:
						_returnData = &uniformBufferMemories;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT:
						_returnData = &descriptorSetLayouts;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL:
						_returnData = &descriptorPools;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET:
						_returnData = &descriptorSets;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COMMAND_POOL:
						_returnData = &commandPools;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COMMAND_BUFFER:
						_returnData = &commandBuffers;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE:
						_returnData = &imageAvailableSemaphores;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED:
						_returnData = &renderFinishedSemaphores;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT:
						_returnData = &inFlightFences;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_QUEUE_GRAPHICS:
						_returnData = &graphicsQueues;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_QUEUE_PRESENT:
						_returnData = &presentQueues;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PUSH_COMMAND:
						// TODO: Push commands
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_ICON:
						// TODO: icon
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT:
						_returnData = &gameObjects;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_OBJECT_FORMAT:
						_returnData = &vulkanFormats;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_OBJECT_DATA_CONTAINER:
						_returnData = &objectDatas;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_MODEL:
						_returnData = &modelPaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_TEXTURE:
						_returnData = &texturePaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_ICON:
						_returnData = &iconPaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_SCRIPT:
						_returnData = &scriptsPaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_ASSET_INDEX_PATH:
						_returnData = &assetIndexPaths;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_GLOBAL:
						_returnData = &global;
						break;
					case DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN:
						_returnData = &unknown;
						break;
					default:
						break;
					}

					return _returnData;
				}

				std::string _typeNames(DMKManagerResourceType type) {
				}
				/* DATA STORE */
				_SET_VECTOR(ADGRVulkanDataContainer) vulkanDataContainers = {};

				_SET_VECTOR(VkInstance) instances = {};
				_SET_VECTOR(VkDevice) devices = {};
				_SET_VECTOR(VkPhysicalDevice) physicalDevices = {};
				_SET_VECTOR(GLFWwindow*) windows = {};
				_SET_VECTOR(VkSurfaceKHR) surfaces = {};
				_SET_VECTOR(VkDebugUtilsMessengerEXT) debugMessengers = {};
				_SET_VECTOR(VkSwapchainKHR) swapchains = {};
				_SET_2D_VECTOR(VkImage) swapchainImages = {};
				_SET_VECTOR(VkFormat) swapchainImageFormats = {};
				_SET_VECTOR(VkExtent2D) swapchainImageExtent = {};
				_SET_VECTOR(VkImageView) swapchainImageViews = {};
				_SET_VECTOR(std::string) shaderPaths = {};
				_SET_2D_VECTOR(VkShaderModule) shaderModules = { {}, {}, {}, {} };
				_SET_2D_VECTOR(VkPipeline) pipelines = {};
				_SET_2D_VECTOR(VkRenderPass) pipelineRenderPasses = { };
				_SET_2D_VECTOR(VkPipelineLayout) pipelineLayouts = {};
				_SET_VECTOR(VkBuffer) colorBuffers = {};
				_SET_VECTOR(VkImage) colorBufferImages = {};
				_SET_VECTOR(VkDeviceMemory) colorBufferImageMemories = {};
				_SET_VECTOR(VkImageView) colorBufferImageViews = {};
				_SET_VECTOR(VkBuffer) depthBuffers = {};
				_SET_VECTOR(VkImage) depthBufferImages = {};
				_SET_VECTOR(VkDeviceMemory) depthBufferImageMemories = {};
				_SET_VECTOR(VkImageView) depthBufferImageViews = {};
				_SET_2D_VECTOR(VkFramebuffer) frameBuffers = {};
				_SET_2D_VECTOR(VkImage) textureImages = {};
				_SET_2D_VECTOR(VkDeviceMemory) textureImageMemories = {};
				_SET_2D_VECTOR(VkImageView) textureImageViews = {};
				_SET_2D_VECTOR(VkSampler) textureImageSamplers = {};
				_SET_2D_VECTOR(Vertex) vertexBufferObjects = {};
				_SET_2D_VECTOR(VkBuffer) vertexBuffers = {};
				_SET_2D_VECTOR(VkDeviceMemory) vertexBufferMemories = {};
				_SET_2D_VECTOR(UI32) indexBufferObjects = {};
				_SET_2D_VECTOR(VkBuffer) indexBuffers = {};
				_SET_2D_VECTOR(VkDeviceMemory) indexBufferMemories = {};
				_SET_2D_VECTOR(VkBuffer) uniformBuffers = {};
				_SET_2D_VECTOR(VkDeviceMemory) uniformBufferMemories = {};
				_SET_VECTOR(VkDescriptorSetLayout) descriptorSetLayouts = {};
				_SET_VECTOR(VkDescriptorPool) descriptorPools = {};
				_SET_2D_VECTOR(VkDescriptorSet) descriptorSets = {};
				_SET_2D_VECTOR(VkCommandPool) commandPools = {};
				_SET_2D_VECTOR(VkCommandBuffer) commandBuffers = {};
				_SET_VECTOR(VkSemaphore) imageAvailableSemaphores = {};
				_SET_VECTOR(VkSemaphore) renderFinishedSemaphores = {};
				_SET_VECTOR(VkFence) inFlightFences = {};
				_SET_2D_VECTOR(VkQueue) graphicsQueues = {};
				_SET_2D_VECTOR(VkQueue) presentQueues = {};

				_SET_VECTOR(DMKObjectData) objectDatas = {};
				_SET_VECTOR(GameObject*) gameObjects = {};
				_SET_VECTOR(vulkanFormat) vulkanFormats = {};

				_SET_VECTOR(std::string) modelPaths = {};
				_SET_VECTOR(std::string) texturePaths = {};
				_SET_VECTOR(std::string) iconPaths = {};
				_SET_VECTOR(std::string) scriptsPaths = {};
				_SET_VECTOR(std::string) assetIndexPaths = {};

				_SET_VECTOR(std::string) typeNames = {
					"ADGRVulkanDataContainer",
					"VkDevice",
					"VkPhysicalDevice",
					"GLFWwindow*",
					"VkSurfaceKHR",
					"VkDebugUtilsMessengerEXT",
					"VkSwapchainKHR",
					"VkImage",
					"VkFormat",
					"VkExtent2D",
				};
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif
