#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RBL_H
#define _DYNAMIK_ADGR_VULKAN_RBL_H

#include "Graphics/VulkanGraphicsCommandBuffer.h"
#include "Graphics/VulkanGraphicsCore.h"
#include "Graphics/VulkanGraphicsPrimitiveManager.h"
#include "Graphics/VulkanGraphicsShader.h"

#include "Common/VulkanDescriptor.h"

#include "Objects/InternalFormat/InternalFormat.h"

#include "../../Components/Attachments/Internal/ColorAttachment.h"
#include "../../Components/Attachments/Internal/DepthAttachment.h"
#include "../Primitives/PrimitiveContainer.h"

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

				/* Basic initializations */
				static void setWindowHandle(POINTER<GLFWwindow> handle);
				static void setWindowExtent(UI32 width, UI32 height);
				static void setProgressPointer(POINTER<UI32> progress);

				/* Core initializations */
				static void initializeInstance();
				static void initializeDevices();
				static void initializeStageOne();

				/* Attachment functions */
				static POINTER<ColorAttachment> generateColorAttachment(POINTER<VulkanSwapChain> swapChain);
				static void destroyColorAttachment(POINTER<ColorAttachment> ptr);
				static POINTER<DepthAttachment> generateDepthAttachment();
				static void destroyDepthAttachment(POINTER<DepthAttachment> ptr);

				/* Context initialization functions */
				static POINTER<VulkanSwapChain> initializeSwapChain();
				static void terminateSwapChain(POINTER<VulkanSwapChain> swapChain);
				static POINTER<VulkanRenderPass> initializeRenderPass();
				static void terminateRenderPass(POINTER<VulkanRenderPass> renderPass);
				static ARRAY<POINTER<VulkanFrameBuffer>> initializeFrameBuffers(POINTER<VulkanSwapChain> swapChain, ARRAY<POINTER<RenderAttachment>> attachments);
				static void terminateFrameBuffers(ARRAY<POINTER<VulkanFrameBuffer>> frameBuffers);

				static RenderContext createContext(RenderContextType type, ARRAY<POINTER<RenderAttachment>> attachments);
				static void destroyContext(RenderContext context);

				/* Resource initialize functions */
				static POINTER<VulkanVertexBuffer> initializeVertexBuffer(const Mesh& mesh, ARRAY<DMKVertexAttribute> attributes);
				static void terminateVertexBuffer(POINTER<VulkanVertexBuffer> buffer);
				static POINTER<VulkanIndexBuffer> initializeIndexBuffer(const Mesh& mesh, DMKDataType indexType);
				static void terminateIndexBuffer(POINTER<VulkanIndexBuffer> buffer);
				static POINTER<VulkanTextureData> initializeTextureData(const Texture& texture);
				static void terminateTextureData(POINTER<VulkanTextureData> texture);

				/* size = Uniform Buffer byte size */
				static VulkanUniformBuffer initializeUniformBuffer(UI32 size);
				static void terminateUniformBuffer(POINTER<VulkanUniformBuffer> buffer);
				static ARRAY<VulkanUniformBuffer> initializeUniformBuffers(UI32 size);
				static void terminateUniformBuffers(ARRAY<POINTER<VulkanUniformBuffer>> buffers);
				static VulkanPipeline initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info);
				static void terminatePipeline(POINTER<VulkanPipeline> pipeline);

				/* Submit objects */
				static void addRenderableToQueue(POINTER<InternalFormat> format);

			private:
				POINTER<GLFWwindow> myWindowHandle;
				ADGRVulkanWindowExtent windowExtent;

				VulkanGraphicsCore myGraphicsCore;
				POINTER<UI32> myProgressPointer;

				VulkanGraphicsCommandBuffer submitPendingCommandBuffer;	/* host visible */
				VulkanGraphicsCommandBuffer inFlightCommandBuffer;	/* client visible */
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RBL_H
