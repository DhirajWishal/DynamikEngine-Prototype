#include "adgrafx.h"
#include "VulkanBRDF.h"

#include "Graphics/VulkanGraphicsFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanBRDF::initialize()
			{
				_initializeTexture();
				_initializeRenderPass();
				_initializeFrameBuffer();
				_initializeDescriptorSetLayout();
				_initializeDescriptorPool();
			}

			void VulkanBRDF::_initializeTexture()
			{
				myTextureContainer.format = VK_FORMAT_R16G16_SFLOAT;
				myTextureContainer.mipLevels = 1;
				myTextureContainer.width = dimentions;
				myTextureContainer.height = dimentions;

				ADGRVulkanCreateImageInfo cinfo;
				cinfo.width = dimentions;
				cinfo.height = dimentions;
				cinfo.format = myTextureContainer.format;
				cinfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				cinfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				cinfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				cinfo.image = &myTextureContainer.image;
				cinfo.imageMemory = &myTextureContainer.imageMemory;
				cinfo.mipLevels = 1;
				cinfo.numSamples = VK_SAMPLE_COUNT_1_BIT;
				cinfo.flags = NULL;
				VulkanGraphicsFunctions::createImage(logicalDevice, physicalDevice, cinfo);

				ADGRVulkanTextureSamplerInitInfo samplerInitInfo;
				samplerInitInfo.magFilter = VK_FILTER_LINEAR;
				samplerInitInfo.minFilter = VK_FILTER_LINEAR;
				samplerInitInfo.maxMipLevels = 1;
				samplerInitInfo.minMipLevels = 0;
				samplerInitInfo.modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				samplerInitInfo.modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				samplerInitInfo.modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				myTextureContainer.imageSampler = VulkanGraphicsFunctions::createImageSampler(logicalDevice, samplerInitInfo);

				ADGRVulkanCreateImageViewInfo cinfo2;
				cinfo2.image = myTextureContainer.image;
				cinfo2.format = myTextureContainer.format;
				cinfo2.mipLevels = myTextureContainer.mipLevels;
				cinfo2.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
				myTextureContainer.imageView = VulkanGraphicsFunctions::createImageView(logicalDevice, cinfo2);
			}

			void VulkanBRDF::_initializeRenderPass()
			{
				VkAttachmentDescription attDesc = {};
				// Color attachment
				attDesc.format = myTextureContainer.format;
				attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
				attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

				VkSubpassDescription subpassDescription = {};
				subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpassDescription.colorAttachmentCount = 1;
				subpassDescription.pColorAttachments = &colorReference;

				// Use subpass dependencies for layout transitions
				std::array<VkSubpassDependency, 2> dependencies;
				dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[0].dstSubpass = 0;
				dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				dependencies[1].srcSubpass = 0;
				dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				ADGRVulkanRenderPassInitInfo initInfo;
				initInfo.attachments = { attDesc };
				initInfo.additionalSubPassDependencies = { dependencies[0], dependencies[1] };
				initInfo.subPasses = { subpassDescription };
				initInfo.overrideDependencies = true;
				myFrameBuffer.initializeRenderPass(logicalDevice, initInfo);
			}
			
			void VulkanBRDF::_initializeFrameBuffer()
			{
				ADGRVulkanGraphicsFrameBufferInitInfo initInfo;
				initInfo.bufferCount = 1;
				initInfo.attachments = { myTextureContainer.imageView };
				initInfo.swapChainExtent = { dimentions , dimentions };
				myFrameBuffer.initializeFrameBuffer(logicalDevice, initInfo);
			}
			
			void VulkanBRDF::_initializeDescriptorSetLayout()
			{
				ADGRVulkanDescriptorSetLayoutInitInfo initInfo;
				myDescriptorContainer.initializeLayout(logicalDevice, initInfo);
			}
			
			void VulkanBRDF::_initializeDescriptorPool()
			{
			}
		}
	}
}