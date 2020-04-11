#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H
#define _DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H

#include "core/Components/TextOverlay.h"
#include "Objects/VulkanCommandBuffer.h"

#include "Objects/VulkanColorBuffer.h"
#include "Objects/VulkanDepthBuffer.h"

#include "External/stb_font_consolas_24_latin1.inl"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			struct ADGRVulkanTextOverlayInitInfo {
				VulkanCore mainCore;

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

				VulkanCore myCoreObject;
				VulkanSwapChain mySwapChainObject;
				ADGRVulkanRenderData myRenderData;
				VulkanColorBuffer colorBuffer;
				VulkanDepthBuffer depthBuffer;

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
				class _localCommandBuffer : public VulkanCommandBuffer
				{
				public:
					_localCommandBuffer() {}
					~_localCommandBuffer() {}

					void initializeCommandBuffers(ADGRVulkanCommandBufferInitInfo info) override;

					UI32 numLetters = 0;
					VkBuffer vertexBuffer = VK_NULL_HANDLE;
				} commandBufferManager;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_TEXT_OVERLAY_H
