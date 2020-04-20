#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H
#define _DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H

#include "core/Engines/ADGR/Backend/Components/TextOverlay.h"
#include "Graphics/VulkanGraphicsCommandBuffer.h"

#include "Graphics/VulkanGraphicsColorBuffer.h"
#include "Graphics/VulkanGraphicsDepthBuffer.h"

#include "External/stb_font_consolas_24_latin1.inl"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanTextOverlayInitInfo {
				VulkanGraphicsCore mainCore;

				UI32 frameBufferWidth = 0;
				UI32 frameBufferHeight = 0;
				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				VkFormat colorFormat;
				VkFormat depthFormat;
			};

			class VulkanTextOverlay : public TextOverlay {
			public:
				VulkanTextOverlay(ADGRVulkanTextOverlayInitInfo info);
				VulkanTextOverlay() {}
				~VulkanTextOverlay() {}

				void setResources(ADGRVulkanTextOverlayInitInfo info);

				void initialize();
				void update(std::string text, F32 x, F32 y, DMKTextAlign align) override;
				void terminate();

				VkCommandBuffer draw(UI32 image);
				VkSwapchainKHR getSwapChain();

				void initializeCommandBuffers();

				B1 isVisible = true;
				B1 isInitialized = false;

				VkRenderPass getRenderPass();

			private:
				void _initializeCommandPool();
				void _initializeSwapChain();
				void _initializeRenderPass();

				void _initializeVertexBuffer();
				void _initializeTextureImage();
				void _initializeDescriptorPool();
				void _initializeDescriptorSetLayout();
				void _initializePipelineLayout();
				void _initializeDescriptorSets();
				void _initializePipelineCache();
				void _initializePipeline();

				void _beginUpdate();
				void _endUpdate();

				void _terminateCommandPool();

				void _terminateVertexBuffer();
				void _terminateTextureImage();
				void _terminateDescriptorPool();
				void _terminateDescriptorLayout();
				void _terminatePipelineLayout();
				void _terminatePipeline();
				void _terminateSwapChain();

				VulkanGraphicsCore myCoreObject;
				VulkanGraphicsSwapChain mySwapChainObject;
				VulkanGraphicsFrameBuffer myFrameBuffer;
				ADGRVulkanRenderData myRenderData;
				VulkanGraphicsColorBuffer colorBuffer;
				VulkanGraphicsDepthBuffer depthBuffer;

				VkPipelineCache pipelineCache = VK_NULL_HANDLE;
				glm::vec4* mapped = nullptr;
				stb_fontchar stbFontData[STB_FONT_consolas_24_latin1_NUM_CHARS];
				UI32 numLetters;
			private:

				std::string vertexShaderPath = "";
				std::string tessellationShaderPath = "";
				std::string geometryShaderPath = "";
				std::string fragmentShaderPath = "";

				ADGRVulkanTextureContainer textureContainer;

				UI32 width, height;

				VkBuffer vertexBuffer;
				VkDeviceMemory vertexBufferMemory;

			private:
				class _localCommandBuffer : public VulkanGraphicsCommandBuffer
				{
				public:
					_localCommandBuffer() {}
					~_localCommandBuffer() {}

					void initializeCommandBuffers(ADGRVulkanGraphicsCommandBufferInitInfo info) override;

					UI32 numLetters = 0;
					VkBuffer vertexBuffer = VK_NULL_HANDLE;
				} commandBufferManager;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H
