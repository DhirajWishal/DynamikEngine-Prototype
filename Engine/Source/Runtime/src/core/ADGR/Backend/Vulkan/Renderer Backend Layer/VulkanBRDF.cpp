#include "dmkafx.h"
#include "VulkanBRDF.h"

#include "Graphics/VulkanGraphicsFunctions.h"
#include "Graphics/VulkanGraphicsOneTimeCommandBuffer.h"

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
				_initializeDescriptorSets();
				_initializePipelineLayout();
				_initializePipeline();
				_initializeCommandBuffers();
			}

			void VulkanBRDF::terminate()
			{
				myPipeline.terminate(logicalDevice);
				myFrameBuffer.terminate(logicalDevice);
				myDescriptorContainer.terminate(logicalDevice);
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
				cinfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
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
				VkDescriptorPoolSize poolSize;
				poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSize.descriptorCount = 1;

				ADGRVulkanDescriptorPoolInitInfo initInfo;
				initInfo.maxDescriptorSets = 1;
				initInfo.poolSizes = { poolSize };
				myDescriptorContainer.initializePool(logicalDevice, initInfo);
			}

			void VulkanBRDF::_initializeDescriptorSets()
			{
				myDescriptorContainer.allocateSets(logicalDevice);
			}

			void VulkanBRDF::_initializePipelineLayout()
			{
				ADGRVulkanGraphicsPipelineLayoutInitInfo initInfo;
				initInfo.layouts = { myDescriptorContainer.layout };
				myPipeline.initializePipelineLayout(logicalDevice, initInfo);
			}

			void VulkanBRDF::_initializePipeline()
			{
				ADGRVulkanGraphicsShaderInitInfo VSinitInfo;
				VSinitInfo.path = "Shaders/BRDF/vert.spv";
				VSinitInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_VERTEX;
				VulkanGraphicsShader vertShader;
				vertShader.initialize(logicalDevice, VSinitInfo);

				ADGRVulkanGraphicsShaderInitInfo FSinitInfo;
				FSinitInfo.path = "Shaders/BRDF/frag.spv";
				FSinitInfo.type = ADGRVulkanGraphicsShaderType::ADGR_VULKAN_SHADER_TYPE_FRAGMENT;
				VulkanGraphicsShader fragShader;
				fragShader.initialize(logicalDevice, FSinitInfo);

				ADGRVulkanGraphicsPipelineInitInfo initInfo;
				initInfo.shaders = { vertShader, fragShader };
				initInfo.vertexAttributeDescription = {};
				initInfo.vertexBindingDescription = {};
				initInfo.swapChainExtent.width = dimentions;
				initInfo.swapChainExtent.height = dimentions;
				initInfo.inputAssemblyTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				initInfo.renderPass = myFrameBuffer.renderPass;
				myPipeline.initializePipeline(logicalDevice, initInfo);
			}

			void VulkanBRDF::_initializeCommandBuffers()
			{
				VulkanGraphicsOneTimeCommandBuffer oneTimeCommandBuffer(logicalDevice, commandPool, graphicsQueue, presentQueue);
				std::array<VkClearValue, 1> clearValues;
				clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = myFrameBuffer.renderPass;
				renderPassInfo.framebuffer = myFrameBuffer.buffers[0];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent.width = myFrameBuffer.frameWidth;
				renderPassInfo.renderArea.extent.height = myFrameBuffer.frameHeight;
				renderPassInfo.clearValueCount = clearValues.size();
				renderPassInfo.pClearValues = clearValues.data();

				VkCommandBuffer buffer = oneTimeCommandBuffer.myCommandBuffers[0];

				vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				VkViewport viewport;
				viewport.height = dimentions;
				viewport.width = dimentions;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				viewport.x = 0;
				viewport.y = 0;
				vkCmdSetViewport(buffer, 0, 1, &viewport);

				VkRect2D scissor;
				scissor.extent.width = dimentions;
				scissor.extent.height = dimentions;
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(buffer, 0, 1, &scissor);

				vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, myPipeline.pipeline);

				vkCmdDraw(buffer, 3, 1, 0, 0);

				vkCmdEndRenderPass(buffer);
			}
		}
	}
}