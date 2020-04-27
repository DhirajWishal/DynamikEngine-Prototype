#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h file
*/

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "../base/RendererBackendBase.h"

#include "GameObject.h"
#include "debugger.h"

#include "Managers/Asset/AssetManager.h"

#include "Renderer Backend Layer/Graphics/VulkanGraphicsRBLIndex.h"
#include "Renderer Backend Layer/Compute/VulkanComputeRBLIndex.h"

#include "Renderer Backend Layer/VulkanObject2D.h"
#include "Renderer Backend Layer/VulkanObject3D.h"
#include "Renderer Backend Layer/VulkanGraphicsSwapChain3D.h"
#include "Renderer Backend Layer/VulkanTextOverlay.h"
#include "Renderer Backend Layer/VulkanPBRObject.h"
#include "Renderer Backend Layer/VulkanSkyBox.h"

#include "Renderer Backend Layer/Animations/VulkanSkeletalAnimation.h"

#include "Renderer Backend Layer/External/stb_font_consolas_24_latin1.inl"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

		enum class ADGRVulkanResourceState {
			ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE,		/* Resource which the application has and not yet submitted */
			ADGR_VULKAN_RESOURCE_STATE_CLIENT_VISIBLE,		/* Resource which is submitted */
		};

		struct ADGRVulkanRenderResourceContainer {
			VulkanGraphicsCommandBuffer commandBuffer;
			ARRAY<ADGRVulkanRenderData> renderData;
			ADGRVulkanResourceState state = ADGRVulkanResourceState::ADGR_VULKAN_RESOURCE_STATE_HOST_VISIBLE;
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
		class vulkanRenderer : public RendererBackend {
			vulkanRenderer() {}

			static vulkanRenderer instance;

		public:
			~vulkanRenderer() {}

			static void setWindowHandle(POINTER<GLFWwindow> windowHandle);
			static void setWindowExtent(UI32 width, UI32 height);
			static void setProgress(POINTER<UI32> progress);

			static void initializeGraphicsCore();
			static void initializeComputeCore();

			static void addObject(POINTER<InternalFormat> format);
			static void addObjects(ARRAY<POINTER<InternalFormat>> formats);

			/* Vulkan Per-Object functions */
			static void initializeVertexBuffers(POINTER<InternalFormat> format);
			static void initializeIndexBuffers(POINTER<InternalFormat> format);
			static void initializeTextures(POINTER<InternalFormat> format);
			static void initializeUniformBuffers(POINTER<InternalFormat> format);
			static void initializePipeline(POINTER<InternalFormat> format);

			static void initializeCommands();
			static void initializeFinalComponents();

			static void drawFrame(DMKRendererDrawFrameInfo info);
			static void recreateSwapChain();

		private:
			void _initializeRenderPass();	/* Initialize the render pass */
			void _prepareRenderDataContainer(UI32 index);	/* Prepare the next container to be used */
			ADGRVulkanGraphicsRenderableObjectInitInfo _getBasicInitInfo();	/* Return the basic init info */

		public:
			/* Initialize the object as a Static object */
			ADGRVulkanRenderData _initializeStaticObject(POINTER<InternalFormat> format);

			/* Initialize the object as a SkyBox */
			ADGRVulkanRenderData _initializeSkyBox(POINTER<InternalFormat> format);

			/* Initialize the object as s Skeletal animation */
			ADGRVulkanRenderData _initializeSkeletalAnimation(POINTER<InternalFormat> format);

		private:
			POINTER<UI32> myProgress;

			POINTER<GLFWwindow> myWindowHandle;	/* Window handle for vulkan to submit frames */
			UI32 windowWidth = 0;	/* Window width */
			UI32 windowHeight = 0;	/* Window height */

			VulkanGraphicsCore myGraphicsCore;	/* Contains the Vulkan's core components */

			/* Contains the command buffers and render datas */
			ADGRVulkanRenderResourceContainer myResourceContainers[2];	/* Host visible and client visible */
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
