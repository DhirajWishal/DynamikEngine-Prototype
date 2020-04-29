#include "dmkafx.h"
#include "VulkanRBL.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanRBL::basicInternalInitialization()
			{
				VulkanGraphicsPrimitiveManager::setCoreData(&instance.myGraphicsCore, VkCommandPool());
			}

			void VulkanRBL::setWindowHandle(POINTER<GLFWwindow> handle)
			{
				instance.myWindowHandle = handle;
			}

			void VulkanRBL::setWindowExtent(UI32 width, UI32 height)
			{
				instance.windowExtent.width = width;
				instance.windowExtent.height = height;
			}

			void VulkanRBL::setProgressPointer(POINTER<UI32> progress)
			{
				instance.myProgressPointer = progress;
			}

			void VulkanRBL::initializeStageOne()
			{
			}

			VulkanSwapChain VulkanRBL::createSwapChain()
			{
				return VulkanSwapChain();
			}

			VulkanRenderPass VulkanRBL::createRenderPass()
			{
				return VulkanRenderPass();
			}

			ARRAY<VulkanFrameBuffer> VulkanRBL::createFrameBuffers()
			{
				return ARRAY<VulkanFrameBuffer>();
			}

			VulkanVertexBuffer VulkanRBL::initializeVertexBuffer(const Mesh& mesh, ARRAY<DMKVertexAttribute> attributes)
			{
				return VulkanGraphicsPrimitiveManager::createVertexBuffer(mesh, attributes);
			}

			VulkanIndexBuffer VulkanRBL::initializeIndexBuffer(const Mesh& mesh, DMKDataType indexType)
			{
				return VulkanGraphicsPrimitiveManager::createIndexBuffer(mesh, indexType);
			}

			VulkanTextureData VulkanRBL::initializeTextureData(const Texture& texture)
			{
				ADGRVulkanTextureInitInfo info;
				info.mipLevels = 1;
				info.minMipLevels = 0;
				info.maxMipLevels = 1;
				info.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.magFilter = VK_FILTER_LINEAR;
				info.minFilter = VK_FILTER_LINEAR;
				info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				return VulkanGraphicsPrimitiveManager::createTextureData(info, texture);
			}

			VulkanUniformBuffer VulkanRBL::initializeUniformBuffer(UI32 size)
			{
				return VulkanGraphicsPrimitiveManager::createUniformBuffer(size);
			}

			VulkanPipeline VulkanRBL::initializePipeline(ADGRVulkanGraphicsPipelineInitInfo info)
			{
				return VulkanGraphicsPrimitiveManager::createPipeline(info);
			}

			void VulkanRBL::addRenderableToQueue(POINTER<InternalFormat> format)
			{
			}
		}
	}
}