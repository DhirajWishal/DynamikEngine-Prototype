#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 VulkanRBL.h file
*/

#ifndef _DYNAMIK_RENDERER_VULKAN_RENDERER_H
#define _DYNAMIK_RENDERER_VULKAN_RENDERER_H

#include "../base/RendererBackendBase.h"
#include "../../Renderer.h"

#include "GameObject.h"
#include "debugger.h"

#include "Managers/Asset/AssetManager.h"

#include "Renderer Backend Layer/Graphics/VulkanGraphicsRBLIndex.h"
#include "Renderer Backend Layer/Compute/VulkanComputeRBLIndex.h"

#include "Renderer Backend Layer/VulkanGraphicsSwapChain3D.h"

#include "Renderer Backend Layer/External/stb_font_consolas_24_latin1.inl"

namespace Dynamik {
	namespace Renderer {
		using namespace Backend;

		enum class VulkanResourceState {
			ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE,		/* Resource which the application has and not yet submitted */
			ADGR_VULKAN_RESOURCE_STATE_CLIENT_VISIBLE,		/* Resource which is submitted */
		};

		struct VulkanRenderResourceContainer {
			VulkanGraphicsCommandBuffer commandBuffer;
			std::vector<VulkanRenderData> renderData;
			VulkanResourceState state = VulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE;
		};

		/* Vulkan Render Context */
		struct VulkanRenderContext {
			DMKRenderContextType type = DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT;

			VulkanGraphicsRenderPass renderPass;
			VulkanSurfaceContainer surfaceContainer;
			VulkanGraphicsSwapChain swapChain;
			VulkanGraphicsFrameBuffer frameBuffer;

			VulkanGraphicsCommandBuffer inFlightCommandBuffer;
			std::vector<VulkanRenderData> renderDatas;
			VulkanResourceState state = VulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE;
		};

		/* RENDERER BACKEND LAYER
		 * BASE: RendererBackend
		 * Vulkan Backend API.
		 * Derived from the RendererBackend super class.
		 * This class contains all the necessary functions, data types and objects to correctly
		  handle all Vulkan-based rendering tasks.
		  *
		  * @warn: Only one instance should be created.
		 */
		class VulkanRBL : public RendererBackend {
		public:
			VulkanRBL() {}
			~VulkanRBL() {}

			void setMsaaSamples(DMKPipelineMSAASamples samples);
			void setWindowHandle(DMKWindowHandle windowHandle);
			void setWindowExtent(UI32 width, UI32 height);
			void setProgress(POINTER<UI32> progress);

			void initializeGraphicsCore();
			void initializeComputeCore();

			/* 
			 Creates a new render context and stores it in the render context container accorfing to the
			 DMKRenderContextType enum.
			*/
			void createNewContext(DMKRenderContextType type, POINTER<GLFWwindow> windowHandle);

			void addObject(POINTER<InternalFormat> format);
			void addObjects(std::vector<POINTER<InternalFormat>> formats);

			/* Returns the memory location of the objects data */
			VPTR initializeObject(POINTER<InternalFormat> format, DMKRenderContextType context = DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT);

			void initializeCommands();
			void initializeFinalComponents();

			void drawFrame(DMKRendererDrawFrameInfo info, DMKRenderContextType context = DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT);
			void recreateSwapChain();

		private:
			inline B1 _validateMSAASamples();
			inline VulkanSurfaceContainer _createNewSurface(POINTER<GLFWwindow> windowHandle);
			inline VulkanRenderPassInitInfo _getDefaultRenderPassInfo(VkFormat swapChainImageFormat);
			inline VulkanGraphicsRenderableObjectInitInfo _getBasicInitInfo();	/* Return the basic init info */

			/* Per object functions */
			inline VulkanBufferContainer createVertexBuffer(Mesh mesh, std::vector<DMKVertexAttribute> attributes);
			inline VulkanBufferContainer createIndexBuffer(Mesh mesh, DMKDataType type);
			inline VulkanTextureContainer createTextureImage(Texture texture);
			inline VulkanUnformBufferContainer createUniformBuffers(DMKUniformBufferObjectDescriptor uniformBufferDescriptor, UI32 bufferCount);
			inline VulkanGraphicsDescriptor createDescriptors(std::vector<DMKUniformBufferObjectDescriptor> descriptors, std::vector<VulkanUnformBufferContainer> uniformBufferContainers, std::vector<VulkanTextureContainer> textureContainers);
			inline VulkanGraphicsPipeline createPipeline(std::vector<VulkanGraphicsDescriptor> descriptors, std::vector<DMKUniformBufferObjectDescriptor> uniformBufferDescriptors, std::vector<DMKVertexAttribute> attributes, ShaderPaths paths, DMKObjectType objectType, VulkanRenderContext context);

		private:
			/* Initialize the object as s Skeletal animation */
			VulkanRenderData _initializeSkeletalAnimation(POINTER<InternalFormat> format);

			/* Store the captured image to an .raw file */
			void _storeData(VulkanTextureContainer container);
			UI32 _captureCount = 0;

		private:
			POINTER<UI32> myProgress;	/* Progress counter */

			POINTER<GLFWwindow> myWindowHandle;	/* Window handle for vulkan to submit frames */
			UI32 windowWidth = 0;	/* Window width */
			UI32 windowHeight = 0;	/* Window height */

			VulkanGraphicsCore myGraphicsCore;	/* Contains the Vulkan's core components */
			VkSampleCountFlagBits myMsaaSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;
			VulkanGraphicsCommandBuffer myMainCommandBuffer;

			/* Contains the command buffers and render datas */
			B1 inUseIndex = false;	/* Resource index of the container which is currently being used (client visible) */

			VulkanGraphicsColorBuffer myColorBuffer;	/* Core Vulkan attachments */
			VulkanGraphicsDepthBuffer myDepthBuffer;	/* Core Vulkan attachments */

			/* Draw call variables */
			UI32 imageIndex = 0;
			UI32 currentFrame = 0;
			VkResult result = VkResult::VK_ERROR_UNKNOWN;

			/* Render context container */
			VulkanSurfaceContainer myBasicSurface = {};		/* Parent window surface */
			std::vector<VulkanRenderContext> myRenderContexts;
			inline void _sortRenderContexts();

			/* Attachment container */
			POINTER<VulkanRenderData> skyboxObject;
		};
	}
}

#endif	//	_DMK_RENDERING_TECHNOLOGY_H
