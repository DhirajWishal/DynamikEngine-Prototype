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

#include "backend/data structures/vertex.h"

#include "backend/functions/textureFunctions.h"

namespace Dynamik {
	namespace ADGR {
		using namespace functions;

		Renderer::Renderer() {
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

			uniformBuffer.initDescriptorSets(descripInfo);

			// command buffer initialization

			DMKBindCommandBufferInfo commandInfo;
			commandInfo.indices = ibo;
			commandInfo.vertexBuffers = { vertexBuffer };
			commandInfo.indexBuffer = indexBuffer;

			myCommandBufferManager.bindCommands(commandInfo);

			// initialize sync objects
			initSyncObjects(myDevice.getDeviceCpy(), &imageAvailableSemaphore,
				&renderFinishedSemaphore, &inFlightFence);
		}

		Renderer::~Renderer() {


			// final
			myWindow.clear();
		}
		void Renderer::loadObjectData()
		{

		}
	}
}
