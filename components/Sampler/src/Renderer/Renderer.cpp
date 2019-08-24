/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "Renderer.h"

#include "Platform/Windows.h"
#include "keyCodes.h"

#include "backend/data structures/vertex.h"

#include "backend/functions/textureFunctions.h"
#include "backend/controlHeader.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace functions;

		Renderer::Renderer() {
		}

		Renderer::~Renderer() {
		}

		void Renderer::init() {
			myWindow.init();

			myInstance.init();
			myDebugger.setupDebugMessenger();

			myWindow.createWindowSurface();

			myDevice.init();

			mySwapChain.init();
			mySwapChain.initImageViews();

			myPipeline.initRenderPass();

			uniformBuffer.createDescriptorSetLayout();
			uniformBuffer.initDescriptorPool();

			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"),
				VERTEX_SHADER);
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"),
				FRAGMENT_SHADER);
			myShaderManager.init();

			myPipeline.init();
			myShaderManager.deleteShaders();

			myCommandBufferManager.initCommandPool();

			// TODO: manually initialization
			myColorBufferManager.initResources();

			// TODO: manually initialization
			myDepthBufferManager.init();

			// TODO: manually initialization
			myFrameBufferManager.init();

			// texture creation - init
			DMKInitTextureInfo textureInfo;
			textureInfo.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/textures/chalet.jpg";
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

			myTextureManager.initTexture(textureInfo);

			// texture - imageViews
			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = 1;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myDevice.getDeviceCpy();
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = 1;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			cImgVewinfo.textureImageView = &textureImageView;

			myTextureManager.initTextureImageViews(viewInfo, cImgVewinfo);

			// texture - sampler
			DMKInitTextureSamplerInfo samplerInfo;
			samplerInfo.textureSampler = &textureSampler;
			samplerInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;

			myTextureManager.initTextureSampler(samplerInfo);

			// model creation

			DMKModelLoadInfo modelInfo;
			modelInfo.path = "E:/Projects/Dynamik Engine/Dynamik/core assets/models/chalet.obj";
			modelInfo.vertices = &vbo;
			modelInfo.indices = &ibo;

			myModelManager.loadModel(modelInfo);

			// vert buff creation
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = &vertexBuffer;
			vertexBufferInfo.buffereMemory = &vertexBufferMemory;
			vertexBufferInfo.vertices = vbo;

			myVertexBufferManager.createVertexBuffer(vertexBufferInfo);

			// index buff creation
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = &indexBuffer;
			indexBufferInfo.buffereMemory = &indexBufferMemory;
			indexBufferInfo.indices = ibo;

			myIndexBufferManager.createIndexBuffer(indexBufferInfo);

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;

			uniformBuffer.createUniformBuffers(unibuffinfo);

			uniformBuffer.initDescriptorPool();

			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;

			uniformBuffer.initDescriptorSets(descripInfo);

			// command buffer initialization

			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { vertexBuffer };
			commandInfo.indexBuffer = indexBuffer;
			commandInfo.descriptorSets = { &descriptorSets };

			myCommandBufferManager.bindCommands(commandInfo);

			// initialize sync objects
			initSyncObjects(myDevice.getDeviceCpy(), &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
		}

		void Renderer::shutdown() {
			// clear color buffer
			myColorBufferManager.clear();

			// clear depth buffer
			myDepthBufferManager.clear();

			// clear swapChain
			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;

			mySwapChain.cleanUp(cleanInfo);

			// clear textures
			DMKTextureDeleteInfo deleteTexInfo;
			deleteTexInfo.sampler = textureSampler;
			deleteTexInfo.texture = texImage;
			deleteTexInfo.imageView = textureImageView;
			deleteTexInfo.textureImageMemory = texImageMemory;

			myTextureManager.deleteTexture(deleteTexInfo);

			// clear buffers
			// clear index buffer
			DMKindexBufferDeleteInfo deleteIndInfo;
			deleteIndInfo.buffer = indexBuffer;
			deleteIndInfo.bufferMemory = indexBufferMemory;

			myIndexBufferManager.deleteBuffer(deleteIndInfo);

			// clear vertex buffer
			DMKVertexBufferDeleteInfo deleteVertInfo;
			deleteVertInfo.buffer = vertexBuffer;
			deleteVertInfo.bufferMemory = vertexBufferMemory;

			myVertexBufferManager.deleteBuffer(deleteVertInfo);

			// delete frames in flight
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(myDevice.getDeviceCpy(), renderFinishedSemaphore[i], nullptr);
				vkDestroySemaphore(myDevice.getDeviceCpy(), imageAvailableSemaphore[i], nullptr);
			}

			// stop the debugger
			myDebugger.deleteDebugger();

			// delete the command pool
			vkDestroyCommandPool(myDevice.getDeviceCpy(), myCommandBufferManager.getCommandPool(), nullptr);

			// delete device and surface
			vkDestroyDevice(myDevice.getDeviceCpy(), nullptr);
			vkDestroySurfaceKHR(myInstance.getInstance(), myInstance.getSurface(), nullptr);

			// clear instance
			myInstance.clear();

			// final
			myWindow.clear();
		}

		void Renderer::drawFrame() {
			vkWaitForFences(myDevice.getDeviceCpy(), 1, &inFlightFence[currentFrame],
				VK_TRUE, std::numeric_limits<uint64_t>::max());

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(myDevice.getDeviceCpy(), mySwapChain.getSwapChainCpy(), std::numeric_limits<uint64_t>::max(),
				imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				std::runtime_error("failed to acquire swap chain image!");

			// draw call

			myEvent = myWindow.getKeyEvent();

			DMKUniformBufferUpdateInfo updateInfo;
			updateInfo.bufferMemory = uniformBufferMemories;
			updateInfo.currentImage = imageIndex;

			std::vector<bool> arr = { myEvent.turnEventL , myEvent.turnEventR };
			std::vector<bool> mov = { myEvent.moveEventU , myEvent.moveEventD };
			updateInfo.turn = arr;
			updateInfo.move = mov;

			uniformBuffer.updateBuffer(updateInfo);

			// others
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = myCommandBufferManager.getBuffer(imageIndex);

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			vkResetFences(myDevice.getDeviceCpy(), 1, &inFlightFence[currentFrame]);

			if (vkQueueSubmit(*myDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFence[currentFrame]) != VK_SUCCESS)
				std::runtime_error("failed to submit draw command buffer!");

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { mySwapChain.getSwapChainCpy() };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			result = vkQueuePresentKHR(*myDevice.getPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindow.isFrameBufferResized()) {
				myWindow.frameBufferResizedUpdate(false);
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				std::runtime_error("failed to present swap chain image!");

			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void Renderer::recreateSwapChain() {
			myWindow.onWindowResizeEvent();

			vkDeviceWaitIdle(myDevice.getDeviceCpy());

			DMKSwapChainCleanUpInfo cleanInfo;
			cleanInfo.uniformBuffers = uniformBuffers;
			cleanInfo.uniformBufferMemories = uniformBufferMemories;

			mySwapChain.cleanUp(cleanInfo);

			mySwapChain.init();
			mySwapChain.initImageViews();

			myPipeline.initRenderPass();

			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/vert.spv"),
				VERTEX_SHADER);
			myShaderManager.loadShader(utils::readFile("E:/Projects/Dynamik Engine/Dynamik/components/Shaders/frag.spv"),
				FRAGMENT_SHADER);
			myShaderManager.init();

			myPipeline.init();
			myShaderManager.deleteShaders();

			// TODO: manually initialization
			myColorBufferManager.initResources();

			// TODO: manually initialization
			myDepthBufferManager.init();

			// TODO: manually initialization
			myFrameBufferManager.init();

			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;

			uniformBuffer.createUniformBuffers(unibuffinfo);

			uniformBuffer.initDescriptorPool();

			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;

			uniformBuffer.initDescriptorSets(descripInfo);

			// command buffer initialization

			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { vertexBuffer };
			commandInfo.indexBuffer = indexBuffer;
			commandInfo.descriptorSets = { &descriptorSets };

			myCommandBufferManager.bindCommands(commandInfo);
		}

		void Renderer::loadObjectData() {
			// texture creation
			DMKInitTextureInfo textureInfo;
			textureInfo.path = "";
			textureInfo.textureImage = &texImage;
			textureInfo.textureImageMemory = &texImageMemory;
			textureInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

			myTextureManager.initTexture(textureInfo);

			DMKInitTextureImageViewsInfo viewInfo;
			viewInfo.textureImage = texImage;
			viewInfo.textureImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.mipLevels = 1;

			DMKCreateImageViewInfo cImgVewinfo;
			cImgVewinfo.device = myDevice.getDeviceCpy();
			cImgVewinfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			cImgVewinfo.image = texImage;
			cImgVewinfo.mipLevels = 1;
			cImgVewinfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

			myTextureManager.initTextureImageViews(viewInfo, cImgVewinfo);
			myTextureManager.initTextureSampler();

			// model creation

			DMKModelLoadInfo modelInfo;
			modelInfo.path = "";
			modelInfo.vertices = &vbo;
			modelInfo.indices = &ibo;

			myModelManager.loadModel(modelInfo);

			// vert buff creation
			DMKVertexBufferCreateInfo vertexBufferInfo;
			vertexBufferInfo.buffer = &vertexBuffer;
			vertexBufferInfo.buffereMemory = &vertexBufferMemory;
			vertexBufferInfo.vertices = vbo;

			myVertexBufferManager.createVertexBuffer(vertexBufferInfo);

			// index buff creation
			DMKIndexBufferCreateInfo indexBufferInfo;
			indexBufferInfo.buffer = &indexBuffer;
			indexBufferInfo.buffereMemory = &indexBufferMemory;
			indexBufferInfo.indices = ibo;

			myIndexBufferManager.createIndexBuffer(indexBufferInfo);

			// uniform buffer creation
			DMKUniformBufferCreateInfo unibuffinfo;
			unibuffinfo.buffers = &uniformBuffers;
			unibuffinfo.bufferMemories = &uniformBufferMemories;

			uniformBuffer.createUniformBuffers(unibuffinfo);

			uniformBuffer.initDescriptorPool();

			DMKDescriptorSetsInitInfo descripInfo;
			descripInfo.uniformBuffers = &uniformBuffers;
			descripInfo.textureImageView = textureImageView;
			descripInfo.textureSampler = textureSampler;
			descripInfo.descriptorSets = &descriptorSets;

			uniformBuffer.initDescriptorSets(descripInfo);
		}
	}
}
