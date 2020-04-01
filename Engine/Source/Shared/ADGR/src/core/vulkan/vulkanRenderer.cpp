/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file
*/

#include "adgrafx.h"
#include "vulkanRenderer.h"
#include "Platform/windows.h"
#include "keyCodes.h"

#include "debugger.h"
#include "CentralDataHub.h"
#include "Platform.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;

#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif

		// initialize the renderer
		void vulkanRenderer::init() {	// 14 inc
			initStageOne();
			initStageTwo();
			initStageThree();
		}

		// Basic one-time initializations
		void vulkanRenderer::initStageOne()
		{
			my3DRenderer.initStageOne();
		}

		// object loading and initialization
		void vulkanRenderer::initStageTwo() {
			my3DRenderer.initStageTwo();
		}

		// final initialization
		void vulkanRenderer::initStageThree() {
			my3DRenderer.initStageThree();
		}

		// shutdown the renderer
		void vulkanRenderer::shutDown() {
			shutDownStageOne();
			shutDownStageTwo();
			shutDownStageThree();
		}

		// basic one-time shut down functions
		void vulkanRenderer::shutDownStageOne() {
		}

		// per object shut down functions
		void vulkanRenderer::shutDownStageTwo() {
			//for (I32 itr = 0; itr < myFormatsCount; itr++) {
			//	vulkanFormat* _localVulkanFormat = &myVulkanFormats[itr];
			//
			//	// clean pipeline and pipeline layout
			//	myPipelineManager.clear(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// clean uniform buffers, uniform buffer memories and descriptor pools
			//	myUniformBufferManager.clean(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// clear textures
			//	myTextureManager.clear(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// clear index buffer
			//	if (_localVulkanFormat->myRendererFormat->myRenderTechnology == DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED)
			//		myIndexBufferManager.clear(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// clear vertex buffer
			//	myVertexBufferManager.clear(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// destroy descriptorSetLayout
			//	vkDestroyDescriptorSetLayout(myVulkanDataContainers[vulkanContainerIndex].device,
			//		_localVulkanFormat->myDescriptorSetLayout, nullptr);
			//}
		}

		// final shut down functions
		void vulkanRenderer::shutDownStageThree() {
			// delete frames in flight
			//for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			//	vkDestroySemaphore(myVulkanDataContainers[vulkanContainerIndex].device, myImageAvailableSemaphores[i], nullptr);
			//	vkDestroySemaphore(myVulkanDataContainers[vulkanContainerIndex].device, myRenderFinishedSemaphores[i], nullptr);
			//	vkDestroyFence(myVulkanDataContainers[vulkanContainerIndex].device, myFencesInFlight[i], nullptr);
			//}
			//
			//// delete device
			//vkDestroyDevice(myVulkanDataContainers[vulkanContainerIndex].device, nullptr);
			//
			//// stop the debugger
			//myDebugger.deleteDebugger();
			//
			//// destroy vulkan surface
			//vkDestroySurfaceKHR(myVulkanDataContainers[vulkanContainerIndex].instance, myVulkanDataContainers[vulkanContainerIndex].surface, nullptr);
			//
			//// clear instance
			//myInstanceManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);
			//
			//// final
			//myWindowManager.clear(&myVulkanDataContainers[vulkanContainerIndex]);
		}

		// draw frame
		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif

			// wait for fences
			//vkWaitForFences(myVulkanDataContainers[vulkanContainerIndex].device, 1, &myFencesInFlight[currentFrame],
			//	VK_TRUE, std::numeric_limits<uint64_t>::max());
			//
			//// get image index
			//imageIndex = 0;
			//result = vkAcquireNextImageKHR(myVulkanDataContainers[vulkanContainerIndex].device,
			//	myVulkanDataContainers[vulkanContainerIndex].swapchainContainer.swapchain, std::numeric_limits<uint64_t>::max(),
			//	myImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
			//
			//// recreate swachain if needed
			//if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			//	recreateSwapChain();
			//	return;
			//}
			//else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			//	DMK_CORE_FATAL("failed to acquire swap chain image!");
			//
			//// draw call
			//// uniform buffer object update
			//for (I32 itr = 0; itr < myFormatsCount; itr++)
			//	myUniformBufferManager.updateBuffer3D(&myVulkanDataContainers[vulkanContainerIndex],
			//		myEventContainers, &myVulkanFormats[itr], imageIndex);
			//
			//// submit info
			//submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			//
			//// wait for semaphores
			//waitSemaphores[0] = { myImageAvailableSemaphores[currentFrame] };
			//waitStages[0] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			//submitInfo.waitSemaphoreCount = 1;
			//submitInfo.pWaitSemaphores = waitSemaphores;
			//submitInfo.pWaitDstStageMask = waitStages;
			//submitInfo.commandBufferCount = 1;
			//submitInfo.pCommandBuffers = &myVulkanDataContainers[vulkanContainerIndex].commandBufferContainer.buffers[imageIndex];
			//
			//// signal semaphores
			//signalSemaphores[0] = { myRenderFinishedSemaphores[currentFrame] };
			//submitInfo.signalSemaphoreCount = 1;
			//submitInfo.pSignalSemaphores = signalSemaphores;
			//
			//// reset fences
			//vkResetFences(myVulkanDataContainers[vulkanContainerIndex].device, 1, &myFencesInFlight[currentFrame]);
			//
			//// submit command queue
			//if (vkQueueSubmit(myVulkanDataContainers[vulkanContainerIndex].graphicsQueue, 1, &submitInfo,
			//	myFencesInFlight[currentFrame]) != VK_SUCCESS)
			//	DMK_CORE_FATAL("failed to submit draw command buffer!");
			//
			//// present queue info
			//presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			//presentInfo.waitSemaphoreCount = 1;
			//presentInfo.pWaitSemaphores = signalSemaphores;
			//
			//// swapchain info
			//swapChains[0] = { myVulkanDataContainers[vulkanContainerIndex].swapchainContainer.swapchain };
			//presentInfo.swapchainCount = 1;
			//presentInfo.pSwapchains = swapChains;
			//presentInfo.pImageIndices = &imageIndex;
			//
			//// submit queue
			//result = vkQueuePresentKHR(myVulkanDataContainers[vulkanContainerIndex].presentQueue, &presentInfo);
			//
			//// frame buffer resize event
			//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindowManager.isFrameBufferResized()) {
			//	myWindowManager.frameBufferResizedUpdate(false);
			//	recreateSwapChain();
			//}
			//else if (result != VK_SUCCESS)
			//	DMK_CORE_FATAL("failed to present swap chain image!");
			//
			//// current frame select
			//currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		// recreate the swapchain
		void vulkanRenderer::recreateSwapChain() {
			//// window resize event
			//myWindowManager.onWindowResizeEvent(&myVulkanDataContainers[vulkanContainerIndex]);
			//
			//// first stage shut down
			//shutDownStageOne();
			//
			//// initialize the command pool
			//initCommandPool();
			//
			//// clear swapChain, Pipeline, Uniform buffers and descriptorPool
			//for (I32 itr = 0; itr < myFormatsCount; itr++) {
			//	vulkanFormat* _localVulkanFormat = &myVulkanFormats[itr];
			//
			//	// clean pipeline and pipeline layout
			//	myPipelineManager.clear(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//
			//	// clean uniform buffers, uniform buffer memories and descriptor pools
			//	myUniformBufferManager.clean(&myVulkanDataContainers[vulkanContainerIndex], _localVulkanFormat);
			//}
			//
			//// init swapchain
			//initSwapChain();
			//
			//// TODO: manually initialization
			//initColorBuffer();
			//
			//// TODO: manually initialization
			//initDepthBuffer();
			//
			//// initialize the render pass
			//initRenderPass();
			//
			//// TODO: manually initialization
			//initFrameBuffers();
			//
			//for (UI32 _itr = 0; _itr < myFormatsCount; _itr++) {
			//	vulkanFormat* _localVulkanFormat = &myVulkanFormats[_itr];
			//
			//	// init pipelines
			//	initPipelines(_localVulkanFormat);
			//
			//	// uniform buffer creation
			//	initUniformBuffers(_localVulkanFormat);
			//
			//	// descriptor pool creation
			//	initDescriptorPoolsAndSets(_localVulkanFormat);
			//}
			//
			//// init command buffers
			//initCommandBuffers(&myVulkanFormats);
		}

		// events
		std::deque<DMKEventContainer>* vulkanRenderer::events() {
			//myWindowManager.pollEvents();
			//myEventContainers = myWindowManager.getEventContainer();
			//
			//return &myEventContainers;
			return nullptr;
		}

		void vulkanRenderer::setFormats(ARRAY<RendererFormat>& rendererFormats) {
			ARRAY<ADGRVulkan3DObjectData> _objectDatas;

			for (UI32 _itr = 0; _itr < rendererFormats.size(); _itr++)
			{
				ADGRVulkan3DObjectData _object;
				_object.vertexShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath;
				_object.tessellationShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath;
				_object.geometryShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath;
				_object.fragmentShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath;

				_object.texturePaths = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.texturePaths.toVector();

				_object.vertexBufferObjects = &rendererFormats[_itr].myInternalFormat->myVertexBufferObjects;
				_object.indexBufferObjects = &rendererFormats[_itr].myInternalFormat->myIndexBufferObjects;

				_objectDatas.push_back(_object);
			}

			my3DRenderer.getObjects(_objectDatas);
		}

		void vulkanRenderer::updateFormats(ARRAY<RendererFormat>& rendererFormats) {
			ARRAY<ADGRVulkan3DObjectData> _objectDatas;

			for (UI32 _itr = 0; _itr < rendererFormats.size(); _itr++)
			{
				ADGRVulkan3DObjectData _object;
				_object.vertexShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath;
				_object.tessellationShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath;
				_object.geometryShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath;
				_object.fragmentShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath;

				_object.texturePaths = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.texturePaths.toVector();

				_object.vertexBufferObjects = &rendererFormats[_itr].myInternalFormat->myVertexBufferObjects;
				_object.indexBufferObjects = &rendererFormats[_itr].myInternalFormat->myIndexBufferObjects;

				_objectDatas.push_back(_object);
			}

			my3DRenderer.getObjects(_objectDatas);
		}
	}
}
#endif