#include "adgrafx.h"
#ifdef DMK_USE_VULKAN
#include "manager.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			//manager::manager() {
			//	shaderModules.resize(4);
			//}
			//
			//template<typename _Type>
			//void manager::addResource(_Type& data, DMKManagerResourceType type) {
			//	_resource<_Type>(type)->push_back(data);
			//}
			//
			//template<typename _Type>
			//_Type& manager::getResource(DMKManagerResourceType type, I32 index) {
			//	return _resource<_Type>(type)->at(index);
			//}
			//
			//template<typename _Type>
			//_Type* manager::_resource(DMKManagerResourceType type) {
			//	_Type* returnData = nullptr;
			//
			//	switch (type) {
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DATA_CONTAINER:
			//		returnData = &vulkanDataContainers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INSTANCE:
			//		returnData = &instances;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEVICE:
			//		returnData = &devices;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PHYSICAL_DEVICE:
			//		returnData = &physicalDevices;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_window:
			//		returnData = &windows;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SURFACE:
			//		returnData = &surfaces;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEBUGGER:
			//		returnData = &debugMessengers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN:
			//		returnData = &swapchains;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGES:
			//		returnData = &swapchainImages;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGE_FORMAT:
			//		returnData = &swapchainImageFormats;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_EXTENT:
			//		returnData = &swapchainImageExtent;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SWAPCHAIN_IMAGE_VIEWS:
			//		returnData = &swapchainImageViews;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_PATH:
			//		returnData = &shaderPaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_VERTEX:
			//		returnData = &shaderModules[0];
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_TESSELLATION:
			//		returnData = &shaderModules[1];
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_GEOMETRY:
			//		returnData = &shaderModules[2];
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SHADER_MODULE_FRAGMENT:
			//		returnData = &shaderModules[3];
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE_RENDER_PASS:
			//		returnData = &pipelineRenderPasses;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE:
			//		returnData = &pipelines;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PIPELINE_LAYOUT:
			//		returnData = &pipelineLayouts;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER:
			//		returnData = &colorBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE:
			//		returnData = &colorBufferImages;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE_MEMORY:
			//		returnData = &colorBufferImageMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COLOR_BUFFER_IMAGE_VIEW:
			//		returnData = &colorBufferImageViews;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER:
			//		returnData = &depthBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE:
			//		returnData = &depthBufferImages;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE_MEMORY:
			//		returnData = &depthBufferImageMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DEPTH_BUFFER_IMAGE_VIEW:
			//		returnData = &depthBufferImageViews;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FRAME_BUFFER:
			//		returnData = &frameBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE:
			//		returnData = &textureImages;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_MEMORY:
			//		returnData = &textureImageMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_SAMPLER:
			//		returnData = &textureImageSamplers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_TEXTURE_IMAGE_VIEW:
			//		returnData = &textureImageViews;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER:
			//		returnData = &vertexBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER_OBJECT:
			//		returnData = &vertexBufferObjects;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_VERTEX_BUFFER_MEMORY:
			//		returnData = &vertexBufferMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER:
			//		returnData = &indexBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER_OBJECT:
			//		returnData = &indexBufferObjects;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_INDEX_BUFFER_MEMORY:
			//		returnData = &indexBufferMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER:
			//		returnData = &uniformBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_UNIFORM_BUFFER_MEMORIES:
			//		returnData = &uniformBufferMemories;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET_LAYOUT:
			//		returnData = &descriptorSetLayouts;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_POOL:
			//		returnData = &descriptorPools;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_DESCRIPTOR_SET:
			//		returnData = &descriptorSets;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COMMAND_POOL:
			//		returnData = &commandPools;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_COMMAND_BUFFER:
			//		returnData = &commandBuffers;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_IMAGE_AVAILABLE:
			//		returnData = &imageAvailableSemaphores;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_SEMAPHORE_RENDER_FINISHED:
			//		returnData = &renderFinishedSemaphores;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_FENCE_IN_FLIGHT:
			//		returnData = &inFlightFences;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_QUEUE_GRAPHICS:
			//		returnData = &graphicsQueues;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_QUEUE_PRESENT:
			//		returnData = &presentQueues;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_VULKAN_PUSH_COMMAND:
			//		// TODO: Push commands
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_ICON:
			//		// TODO: icon
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_GAME_OBJECT:
			//		returnData = &gameObjects;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_OBJECT_DATA_CONTAINER:
			//		returnData = &objectDatas;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_RENDER_DATA_CONTAINER:
			//		returnData = &renderDatas;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_MODEL:
			//		returnData = &modelPaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_TEXTURE:
			//		returnData = &texturePaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_ICON:
			//		returnData = &iconPaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_SCRIPT:
			//		returnData = &scriptsPaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_PATH_ASSET_INDEX_PATH:
			//		returnData = &assetIndexPaths;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_GLOBAL:
			//		returnData = &global;
			//		break;
			//	case DMK_CDH_MANAGER_RESOURCE_TYPE_UNKNOWN:
			//		returnData = &unknown;
			//		break;
			//	default:
			//		break;
			//	}
			//
			//	return returnData;
			//}
		}
	}
}
#endif // DMK_USE_VULKAN
