#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RBL_H
#define _DYNAMIK_ADGR_VULKAN_RBL_H

#include "Graphics/VulkanGraphicsCommandBuffer.h"
#include "Graphics/VulkanGraphicsCore.h"
#include "Graphics/VulkanGraphicsPrimitiveManager.h"
#include "Graphics/VulkanGraphicsShader.h"

#include "Common/VulkanDescriptor.h"

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			/* This acts as the Mesh interpretation in Vulkan */
			struct ADGRVulkanRenderData {
				VulkanVertexBuffer vertexBuffer;
				VulkanIndexBuffer indexBuffer;

				VulkanDescriptor descriptor;
			};

			struct ADGRVulkanRenderContext {
				VulkanSwapChain swapCain;
				VulkanFrameBuffer frameBuffer;
				VulkanGraphicsCommandBuffer commandBuffer;

				ARRAY<ADGRVulkanRenderData> renderData;
			};

			struct ADGRVulkanWindowExtent {
				UI32 width = 0;
				UI32 height = 0;
			};

			class VulkanRBL {
				VulkanRBL() {}
				~VulkanRBL() {}

				static VulkanRBL instance;
			public:
				static void basicInternalInitialization();

				static void setWindowHandle(POINTER<GLFWwindow> handle);
				static void setWindowExtent(UI32 width, UI32 height);
				static void setProgressPointer(POINTER<UI32> progress);

				static void initializeStageOne();

				/* Render Context functions */
				static VulkanSwapChain createSwapChain();
				static VulkanRenderPass createRenderPass();
				static ARRAY<VulkanFrameBuffer> createFrameBuffers();

				/* Per object functions */
				static VulkanVertexBuffer initializeVertexBuffer(const Mesh& mesh, ARRAY<DMKVertexAttribute> attributes);
				static VulkanIndexBuffer initializeIndexBuffer(const Mesh& mesh, DMKDataType indexType);
				static VulkanTextureData initializeTextureData(const Texture& texture);

				/* size = Uniform Buffer byte size */
				static VulkanUniformBuffer initializeUniformBuffer(UI32 size);
				static VulkanPipeline initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info);

				static void addRenderableToQueue(POINTER<InternalFormat> format);

			private:
				POINTER<GLFWwindow> myWindowHandle;
				ADGRVulkanWindowExtent windowExtent;

				VulkanGraphicsCore myGraphicsCore;
				POINTER<UI32> myProgressPointer;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RBL_H
