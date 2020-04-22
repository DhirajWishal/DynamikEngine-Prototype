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

#include "Engines/ADGR/rendererFormat.h"

namespace Dynamik {
	namespace ADGR {
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
			void drawFrame(DMKRendererDrawFrameInfo info);

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
			 * Check for window close event.
			 */
			B1 closeEvent() override;

			/* FUNCTION
			 * Set Renderer formats to the renderer.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void setFormats(ARRAY<POINTER<InternalFormat>>& rendererFormats);
			void setFormats3D(ARRAY<POINTER<InternalFormat>>& rendererFormats);

			/* FUNCTION
			 * Update the renderer formats.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void updateFormats3D(ARRAY<POINTER<InternalFormat>>& rendererFormats);

			void addText(std::string string, F32 x, F32 y, DMKTextAlign align);

			void setWindowHandle(POINTER<GLFWwindow> windowHandle);

			/* PRIVATE FUNCTIONS */
		private:
			void recreateSwapChain();
			void initializeObjects();
			void initializeObjectsBasic();

			void initializeMaterials();

			void initializeOverlay();
			void addOverlay();

			ADGRVulkanGraphicsRenderableObjectInitInfo RenderableObjectInitInfo();

			void initializeSwapChain();

			void initializeColorBuffer();
			void initializeDepthBuffer();

			ADGRVulkanRenderData initializeSkyboxObject(POINTER<InternalFormat> _object);
			ADGRVulkanRenderData initializeReflectObject(POINTER<InternalFormat> _object);
			ADGRVulkanRenderData initializeSkeletalAnimation(POINTER<InternalFormat> _object);

			void draw3D(VkSwapchainKHR swapChain);

			POINTER<GLFWwindow> myWindow;

			VulkanGraphicsCore myVulkanGraphicsCore;
			VulkanGraphicsSwapChain3D mySwapChain3D;
			VulkanGraphicsCommandBuffer myCommandBuffer;
			VulkanGraphicsFrameBuffer myFrameBuffer;

			VulkanGraphicsColorBuffer myColorBuffer;
			VulkanGraphicsDepthBuffer myDepthBuffer;

			ARRAY<ADGRVulkanRenderData> renderDatas;
			ARRAY<ADGRVulkanSkeletalAnimationData> animationDatas;
			ARRAY<POINTER<InternalFormat>> rawObjects;

			ARRAY<VulkanSkyBox> mySkyboxes;

			ADGRVulkanSkeletalAnimationData _skeletalData;
			VulkanSkeletalAnimation* myAnimation;

			WindowManager myWindowManager;

			UI32 currentFrame = 0;
			UI32 imageIndex = 0;
			VkResult result = VkResult::VK_ERROR_UNKNOWN;

			F32 runningTime = 1.0f;

			std::map<std::string, ADGRVulkanMaterialDescriptor> myRenderableMeterials;

		private:
			void initializeComputeAPI();
			VulkanComputeCore myComputeCore;
			VulkanComputeCommandBuffer myComputeCommandBuffer;

		private:
			ADGRVulkanTextOverlayDataContainer overlayContainer;
			VulkanGraphicsCommandBuffer overlayCommandBuffer;
			void _initializeOverlayCommandPool();
			void _initializeOverlayDescriptorSetLayout();
			void _initializeOverlayPipelineLayout();
			void _initializeOverlayRenderPass();

			void _initializeOverlayVertexBuffer();
			void _initializeOverlayTextureImage();
			void _initializeOverlayPipeline(ADGRVulkanGraphicsShaderPathContainer shaderContainer);
			void _initializeOverlayDescriptorPool();
			void _initializeOverlayDescriptorSets();

			VkCommandBuffer _drawOverlay(UI32 imageIndex);

			void _beginUpdate();
			void _endUpdate();

			void _initializeOverlayCommandBuffers(ADGRVulkanGraphicsCommandBufferInitInfo info);

			void _initializeOverlayStageOne();
			void _initializeOverlayStageTwo(ADGRVulkanGraphicsShaderPathContainer shaderContainer);
			void _terminateOverlay();
		};
	}
}

#endif	//	_DMK_RENDERING_TECHNOLOGY_H
