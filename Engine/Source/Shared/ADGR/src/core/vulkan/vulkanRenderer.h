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

#include "core/base/RendererBackendBase.h"

#include "GameObject.h"
#include "debugger.h"

#include "core/data store/internalFormat.h"

#include "Renderer Backend Layer/Objects/VulkanRBLIndex.h"

#include "Renderer Backend Layer/VulkanObject2D.h"
#include "Renderer Backend Layer/VulkanObject3D.h"
#include "Renderer Backend Layer/VulkanSwapChain3D.h"
#include "Renderer Backend Layer/VulkanTextOverlay.h"

#include "core/Window/Windows/WindowManager.h"

#include "core/Components/AnimationCamera.h"
#include "core/Components/Camera2D.h"
#include "core/Components/Camera3D.h"
#include "core/Components/CameraReflect.h"
#include "core/Components/CameraSkybox.h"

#include "Renderer Backend Layer/External/stb_font_consolas_24_latin1.inl"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		using namespace Backend;

		struct ADGRVulkanTextOverlayDataContainer {
			VkBuffer vertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
			ADGRVulkanTextureContainer textureContainer;

			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
			VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

			VkRenderPass renderPass = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

			glm::vec4* mapped = nullptr;
			stb_fontchar stbFontData[STB_FONT_consolas_24_latin1_NUM_CHARS];
			UI32 numberOfLetters = 0;

			B1 isInitialized = false;
			B1 isVisible = true;
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
			/* DEFAULTS */
		public:
			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 */
			vulkanRenderer() {}

			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 *
			 * @param settings: Renderer Backend settings container.
			 */
			vulkanRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}

			/* DESTRUCTOR
			 * Default destructor of the Vulkan Renderer.
			 */
			~vulkanRenderer() {}

			/* PUBLIC FUNCTIONS */
		public:
			/* FUNCTION
			 * Full initialization function.
			 */
			void init() override;

			/* FUNCTION
			 * Stage One initialization.
			 */
			void initStageOne() override;

			/* FUNCTION
			 * Stage Two initialization.
			 */
			void initStageTwo() override;

			/* FUNCTION
			 * Stage Three initialization.
			 */
			void initStageThree() override;

			/* FUNCTION
			 * Draw frame function.
			 */
			void drawFrame();

			/* FUNCTION
			 * Full Shut down function.
			 */
			void shutDown() override;

			/* FUNCTION
			 * Shut down stage one.
			 */
			void shutDownStageOne() override;

			/* FUNCTION
			 * Shut down stage two.
			 */
			void shutDownStageTwo() override;

			/* FUNCTION
			 * Shut down stage three.
			 */
			void shutDownStageThree() override;

			/* FUNCTION
			 * Get events.
			 */
			std::deque<DMKEventContainer>* events() override;

			/* FUNCTION
			 * Check for window close event.
			 */
			B1 closeEvent() override;

			/* FUNCTION
			 * Set Renderer formats to the renderer.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void setFormats(ARRAY<RendererFormat>& rendererFormats);
			void setFormats3D(ARRAY<RendererFormat>& rendererFormats) override;

			/* FUNCTION
			 * Update the renderer formats.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void updateFormats3D(ARRAY<RendererFormat>& rendererFormats) override;

			void addText(std::string string, F32 x, F32 y, DMKTextAlign align);

			/* PRIVATE FUNCTIONS */
		private:
			void recreateSwapChain();
			void initializeObjects();
			void initializeObjectsBasic();

			void initializeOverlay();
			void addOverlay();

			std::pair<ARRAY<Vertex>, ARRAY<UI32>> _skyBoxTest();

			ADGRVulkanRenderableObjectInitInfo RenderableObjectInitInfo();

			void initializeSwapChain();

			void initializeColorBuffer();
			void initializeDepthBuffer();

			ADGRVulkanRenderData initializeSkyboxObject(ADGRVulkan3DObjectData _object);
			ADGRVulkanRenderData initializeReflectObject(ADGRVulkan3DObjectData _object);
			ADGRVulkanRenderData initializeSkeletalAnimation(ADGRVulkan3DObjectData _object);

			void draw3D(VkSwapchainKHR swapChain);

			AnimationCamera myAnimationCamera;
			Camera2D myCamera2D;
			Camera3D myCamera3D;
			CameraReflect myCameraReflect;
			CameraSkybox myCameraSkybox;

			VulkanCore myVulkanCore;
			VulkanSwapChain3D mySwapChain3D;
			VulkanCommandBuffer myCommandBuffer;

			VulkanColorBuffer myColorBuffer;
			VulkanDepthBuffer myDepthBuffer;

			ARRAY<ADGRVulkanRenderData> renderDatas;
			ARRAY<ADGRVulkan3DObjectData> rawObjects;

			WindowManager myWindowManager;

			std::deque<DMKEventContainer> eventContainer;

			UI32 currentFrame = 0;
			UI32 imageIndex = 0;
			VkResult result = VkResult::VK_ERROR_UNKNOWN;

			F32 runningTime = 0.0f;

		private:
			ADGRVulkanTextOverlayDataContainer overlayContainer;
			VulkanCommandBuffer overlayCommandBuffer;
			void _initializeOverlayCommandPool();
			void _initializeOverlayDescriptorSetLayout();
			void _initializeOverlayPipelineLayout();
			void _initializeOverlayRenderPass();

			void _initializeOverlayVertexBuffer();
			void _initializeOverlayTextureImage();
			void _initializeOverlayPipeline(ADGRVulkanShaderPathContainer shaderContainer);
			void _initializeOverlayDescriptorPool();
			void _initializeOverlayDescriptorSets();

			VkCommandBuffer _drawOverlay(UI32 imageIndex);

			void _beginUpdate();
			void _endUpdate();

			void _initializeOverlayCommandBuffers(ADGRVulkanCommandBufferInitInfo info);

			void _initializeOverlayStageOne();
			void _initializeOverlayStageTwo(ADGRVulkanShaderPathContainer shaderContainer);
			void _terminateOverlay();
		};
	}
}

#endif	//	_DMK_ADGR_RENDER_H
