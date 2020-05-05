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
			ARRAY<VulkanRenderData> renderData;
			VulkanResourceState state = VulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE;
		};

		struct VulkanRenderContext {
			VulkanGraphicsSwapChain swapChain;
			VulkanGraphicsFrameBuffer frameBuffer;

			ARRAY<VulkanRenderData> renderDatas;

			VulkanGraphicsCommandBuffer inFlightCommandBuffer;
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

			void addObject(POINTER<InternalFormat> format);
			void addObjects(ARRAY<POINTER<InternalFormat>> formats);

			/* Returns the memory location of the objects data */
			VPTR initializeObject(POINTER<InternalFormat> format);

			void initializeCommands();
			void initializeFinalComponents();

			void drawFrame(DMKRendererDrawFrameInfo info);
			void recreateSwapChain();

		private:
			void _initializeRenderPass();	/* Initialize the render pass */
			void _prepareRenderDataContainer(UI32 index);	/* Prepare the next container to be used */
			VulkanGraphicsRenderableObjectInitInfo _getBasicInitInfo();	/* Return the basic init info */

			/* Per object functions */
			VulkanBufferContainer createVertexBuffer(Mesh mesh, ARRAY<DMKVertexAttribute> attributes);
			VulkanBufferContainer createIndexBuffer(Mesh mesh, DMKDataType type);
			VulkanTextureContainer createTextureImage(Texture texture);
			VulkanUnformBufferContainer createUniformBuffers(DMKUniformBufferObjectDescriptor uniformBufferDescriptor);
			VulkanGraphicsDescriptor createDescriptors(ARRAY<DMKUniformBufferObjectDescriptor> descriptors, ARRAY<VulkanUnformBufferContainer> uniformBufferContainers, ARRAY<VulkanTextureContainer> textureContainers);
			VulkanGraphicsPipeline createPipeline(ARRAY<VulkanGraphicsDescriptor> descriptors, ARRAY<DMKUniformBufferObjectDescriptor> uniformBufferDescriptors, ARRAY<DMKVertexAttribute> attributes, ShaderPaths paths, DMKObjectType objectType);

		public:
			/* Initialize the object as s Skeletal animation */
			VulkanRenderData _initializeSkeletalAnimation(POINTER<InternalFormat> format);

		private:
			POINTER<UI32> myProgress;	/* Progress counter */

			POINTER<GLFWwindow> myWindowHandle;	/* Window handle for vulkan to submit frames */
			UI32 windowWidth = 0;	/* Window width */
			UI32 windowHeight = 0;	/* Window height */

			VulkanGraphicsCore myGraphicsCore;	/* Contains the Vulkan's core components */
			VkSampleCountFlags myMsaaSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
			VulkanGraphicsCommandBuffer myMainCommandBuffer;

			/* Contains the command buffers and render datas */
			VulkanRenderResourceContainer myResourceContainers[2];	/* Host visible and client visible */
			B1 inUseIndex = false;	/* Resource index of the container which is currently being used (client visible) */

			VulkanGraphicsSwapChain3D mySwapChain;	/* 3D Swap Chain */

			VulkanGraphicsColorBuffer myColorBuffer;	/* Core Vulkan attachments */
			VulkanGraphicsDepthBuffer myDepthBuffer;	/* Core Vulkan attachments */

			VulkanGraphicsFrameBuffer myFrameBuffer;	/* Graphics frame buffer */

			/* Draw call variables */
			UI32 imageIndex = 0;
			UI32 currentFrame = 0;
			VkResult result = VkResult::VK_ERROR_UNKNOWN;

			/* Attachment container */
		};
	}
}

#endif	//	_DMK_RENDERING_TECHNOLOGY_H
