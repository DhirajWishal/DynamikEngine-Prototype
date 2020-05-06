/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 VulkanRBL.cpp file
*/

#include "dmkafx.h"
#include "VulkanRBL.h"
#include "Platform/windows.h"

#include "Renderer Backend Layer/Compute/VulkanComputeCore.h"

#include "Renderer Backend Layer/Common/VulkanUtilities.h"
#include "VulkanPresets.h"
#include "Renderer Backend Layer/Common/VulkanControlHeader.h"

#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

/* IDEAS
 Make one time compute objects so that we can compute something at a given instance
 Make multiple of those to compute multiple things.
*/

namespace Dynamik {
	namespace Renderer {
#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif
		void VulkanRBL::setMsaaSamples(DMKPipelineMSAASamples samples)
		{
		}

		void VulkanRBL::setWindowHandle(DMKWindowHandle windowHandle)
		{
#ifdef DMK_PLATFORM_WINDOWS
			WindowsWindow _window = *(WindowsWindow*)windowHandle.get();
			myWindowHandle = _window.window;

#endif // DMK_PLATFORM_WINDOWS
		}

		void VulkanRBL::setWindowExtent(UI32 width, UI32 height)
		{
			windowWidth = width;
			windowHeight = height;
		}

		void VulkanRBL::setProgress(POINTER<UI32> progress)
		{
			myProgress = progress;
		}

		void VulkanRBL::initializeGraphicsCore()
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Initialize the instance */
			VulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myGraphicsCore.initializeInstance(instanceInitInfo);

			/* Initialize the window surface */
			myGraphicsCore.initializeSurface(myWindowHandle);

			/* Initialize the device (physical and logical) */
			myGraphicsCore.initializeDevice();

			/* Initialize command pools (both host visible and client visible) */
			VulkanGraphicsCommandBufferInitResources commandBufferResourceInitInfo;
			commandBufferResourceInitInfo.logicalDevice = myGraphicsCore.logicalDevice;
			commandBufferResourceInitInfo.physicalDevice = myGraphicsCore.physicalDevice;
			commandBufferResourceInitInfo.surface = myGraphicsCore.surface;
			myResourceContainers[0].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			myResourceContainers[0].commandBuffer.initializeCommandPool();
			myResourceContainers[1].commandBuffer.initializeResources(commandBufferResourceInitInfo);
			myResourceContainers[1].commandBuffer.initializeCommandPool();
			myMainCommandBuffer.initializeResources(commandBufferResourceInitInfo);
			myMainCommandBuffer.initializeCommandPool();

			/* Initialize the SwapChain */
			mySwapChain.basicInitialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myGraphicsCore.surface,
				myGraphicsCore.surfaceCapabilities);
			mySwapChain.initializeSwapChain(windowWidth, windowHeight);

			/* Initialize Render pass */
			_initializeRenderPass();

			/* Initialize attachments */
			/* Initialize color buffer */
			myColorBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myResourceContainers[inUseIndex].commandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				mySwapChain.swapChainContainer.swapChainImageFormat,
				mySwapChain.swapChainContainer.swapChainExtent,
				myGraphicsCore.msaaSamples);

			/* Initialize color buffer */
			myDepthBuffer.initialize(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				myResourceContainers[inUseIndex].commandBuffer.pool,
				myGraphicsCore.graphicsQueue,
				myGraphicsCore.presentQueue,
				mySwapChain.swapChainContainer.swapChainExtent,
				myGraphicsCore.msaaSamples);

			/* Initialize Frame Buffer */
			VulkanGraphicsFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.attachments.pushBack(myColorBuffer.imageView);
			frameBufferInitInfo.attachments.pushBack(myDepthBuffer.imageView);
			frameBufferInitInfo.swapChainImageViews = mySwapChain.swapChainContainer.swapChainImageViews;
			frameBufferInitInfo.bufferCount = mySwapChain.swapChainContainer.swapChainImages.size();
			frameBufferInitInfo.swapChainExtent = mySwapChain.swapChainContainer.swapChainExtent;
			myFrameBuffer.initializeFrameBuffer(myGraphicsCore.logicalDevice, frameBufferInitInfo);

			/* Initialize the overlay */
			/* TODO: Overlay */
		}

		void VulkanRBL::initializeComputeCore()
		{
		}

		void VulkanRBL::addObject(POINTER<InternalFormat> format)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Check the object type of the format and initialize it accordingly */
			switch (format->type)
			{
				/* 2D image initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
				break;

				/* Debug object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
				break;

				/* Mesh object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
				break;

				/* Static object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
				break;

				/* Interavtive object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
				break;

				/* Player object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
				break;

				/* Non Player Character object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
				break;

				/* Texture UI object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
				break;

				/* SkyBox object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
				break;

				/* Sprite sheet object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
				break;

				/* Font sheet object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
				break;

				/* Text overlay initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
				break;

				/* Particle system initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
				break;

				/* Destruction entity initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
				break;

				/* Destructor entity initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
				break;

				/* Skeletal animation object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
				break;

				/* Light object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
				break;

				/* Camera object initialization */
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
				break;
			default:
				DMK_CORE_FATAL("Invalid Object Type!");
				break;
			}
		}

		void VulkanRBL::addObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			for (auto format : formats)
				if (format.isValid())
					initializeObject(format);
		}

		VPTR VulkanRBL::initializeObject(POINTER<InternalFormat> format)
		{
			DMK_BEGIN_PROFILE_TIMER();

			VulkanRenderData _renderData;
			_renderData.renderTechnology = format->descriptor.renderSpecification.renderingTechnology;
			_renderData.indexBufferType = format->descriptor.indexBufferType;

			/* Initialize mesh data */
			for (auto mesh : format->meshDatas)
			{
				/* initialize vertex buffers */
				_renderData.vertexBufferContainer.pushBack(createVertexBuffer(mesh, format->descriptor.vertexBufferObjectDescription.attributes));

				/* initialize index buffers */
				_renderData.indexBufferContainer.pushBack(createIndexBuffer(mesh, format->descriptor.indexBufferType));

				/* initialize textures */
				for (UI32 i = 0; i < mesh.textureDatas.size(); i++)
					_renderData.textures.pushBack(createTextureImage(mesh.textureDatas[i]));
			}

			/* Initialize uniform buffers */
			for (auto _uniformBufferDescription : format->descriptor.uniformBufferObjectDescriptions)
				if (_uniformBufferDescription.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
					_renderData.uniformBufferContainers.pushBack(createUniformBuffers(_uniformBufferDescription));

			/* Initialize Descriptors */
			_renderData.descriptors = createDescriptors(format->descriptor.uniformBufferObjectDescriptions, _renderData.uniformBufferContainers, _renderData.textures);

			/* Initialize pipeline */
			_renderData.pipelineContainers.pushBack(createPipeline({ _renderData.descriptors }, format->descriptor.uniformBufferObjectDescriptions, format->descriptor.vertexBufferObjectDescription.attributes, format->shaderPaths, format->type));

			/* Add data to the container and return its address */
			myResourceContainers[inUseIndex].renderData.pushBack(_renderData);
			return &myResourceContainers[inUseIndex].renderData[myResourceContainers[inUseIndex].renderData.size() - 1];
		}

		void VulkanRBL::initializeCommands()
		{
			DMK_BEGIN_PROFILE_TIMER();

			VulkanGraphicsCommandBufferInitInfo initInfo;
			initInfo.count = mySwapChain.swapChainContainer.swapChainImages.size();
			initInfo.frameBuffer = myFrameBuffer;
			initInfo.swapChain = mySwapChain.swapChainContainer;
			initInfo.objects = myResourceContainers[inUseIndex].renderData;

			myResourceContainers[inUseIndex].commandBuffer.initializeCommandBuffers(initInfo);
		}

		void VulkanRBL::initializeFinalComponents()
		{
			DMK_BEGIN_PROFILE_TIMER();

			myGraphicsCore.initializeSyncObjects();
		}

		void VulkanRBL::drawFrame(DMKRendererDrawFrameInfo info)
		{
			DMK_BEGIN_PROFILE_TIMER();

			/* Check if the number of update objects are equal to the in-flight objects */
			if (info.formats.size() != myResourceContainers[inUseIndex].renderData.size())
				DMK_CORE_FATAL("Invalid amount of update formats sent to the Draw call!");

			/* Sync Vulkan Fences */
			myGraphicsCore.syncFence(currentFrame);

			/* Get the current image index */
			imageIndex = 0;
			result = myGraphicsCore.getNextImage(mySwapChain.swapChainContainer.swapChain, &imageIndex, currentFrame);

			/* Check if any errors were encountered */
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				DMK_CORE_FATAL("Failed to acquire Swap Chain image!");

			/* Update the objects using the Draw Frame Info structure */
			for (UI32 index = 0; index < info.formats.size(); index++)
			{
				DMK_BEGIN_PROFILE_TIMER();

				/* Check for the Uniform buffer attributes and add the data to the container */
				for (UI32 _itr = 0; _itr < info.formats[index]->descriptor.uniformBufferObjectDescriptions.size(); _itr++)
				{
					DMK_BEGIN_PROFILE_TIMER();

					/* Currently the Vulkan RBL supports vertex shader uniform buffer updation only */
					if (info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr].location != DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX)
						continue;

					/* Update the objects uniform buffer memory */
					VulkanUtilities::updateUniformBuffer(
						myGraphicsCore.logicalDevice,
						info.formats[index]->onUpdate(info.cameraData),
						myResourceContainers[inUseIndex].renderData[index].uniformBufferContainers[_itr].bufferMemories[imageIndex],
						info.formats[index]->descriptor.uniformBufferObjectDescriptions[_itr]);
				}
			}

			/* Submit queues */
			result = myGraphicsCore.submitQueues(
				{ mySwapChain.swapChainContainer.swapChain },
				imageIndex, currentFrame,
				{ myResourceContainers[inUseIndex].commandBuffer.buffers[imageIndex] });

			/* Check for any errors */
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				recreateSwapChain();
			}
			else if (result != VK_SUCCESS)
				DMK_CORE_FATAL("failed to present swap chain image!");

			/* Update the current frame number */
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void VulkanRBL::recreateSwapChain()
		{
		}

		void VulkanRBL::_initializeRenderPass()
		{
			DMK_BEGIN_PROFILE_TIMER();

			ARRAY<VkAttachmentDescription> attachments;

			// attachment descriptions
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = mySwapChain.swapChainContainer.swapChainImageFormat;
			colorAttachment.samples = myGraphicsCore.msaaSamples;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachments.push_back(colorAttachment);

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			// sub passes
			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkAttachmentDescription depthAttachment = {};
			depthAttachment.format = VulkanUtilities::findDepthFormat(myGraphicsCore.physicalDevice);
			depthAttachment.samples = myGraphicsCore.msaaSamples;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			attachments.push_back(depthAttachment);

			VkAttachmentReference depthAttachmentRef = {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			subpass.pDepthStencilAttachment = &depthAttachmentRef;

			VkAttachmentDescription colorAttachmentResolve = {};
			colorAttachmentResolve.format = mySwapChain.swapChainContainer.swapChainImageFormat;
			colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachments.push_back(colorAttachmentResolve);

			VkAttachmentReference colorAttachmentResolveRef = {};
			colorAttachmentResolveRef.attachment = 2;
			colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			subpass.pResolveAttachments = &colorAttachmentResolveRef;

			ARRAY<VkSubpassDescription> subPasses;
			subPasses.push_back(subpass);

			VulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.attachments = attachments;
			renderPassInitInfo.subPasses = subPasses;
			myFrameBuffer.initializeRenderPass(myGraphicsCore.logicalDevice, renderPassInitInfo);
		}

		void VulkanRBL::_prepareRenderDataContainer(UI32 index)
		{
			myResourceContainers[index].commandBuffer.terminateCommandBuffers();
			myResourceContainers[index].renderData = {};
		}

		VulkanGraphicsRenderableObjectInitInfo VulkanRBL::_getBasicInitInfo()
		{
			VulkanGraphicsRenderableObjectInitInfo _info;
			_info.logicalDevice = myGraphicsCore.logicalDevice;
			_info.physicalDevice = myGraphicsCore.physicalDevice;
			_info.commandPool = myResourceContainers[inUseIndex].commandBuffer.pool;
			_info.graphicsQueue = myGraphicsCore.graphicsQueue;
			_info.presentQueue = myGraphicsCore.presentQueue;

			return _info;
		}

		VulkanBufferContainer VulkanRBL::createVertexBuffer(Mesh mesh, ARRAY<DMKVertexAttribute> attributes)
		{
			VulkanBufferContainer _container;
			_container.dataCount = mesh.vertexDataStore.size();

			VkDeviceSize bufferSize = mesh.allocatableSize(attributes);
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			VulkanCreateBufferInfo bufferInfo;
			bufferInfo.bufferSize = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			bufferInfo.buffer = &stagingBuffer;
			bufferInfo.bufferMemory = &stagingBufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, bufferInfo);

			void* data = nullptr;
			vkMapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
			mesh.packData(attributes, data);
			vkUnmapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory);

			VulkanCreateBufferInfo vertBufferInfo;
			vertBufferInfo.bufferSize = bufferSize;
			vertBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			vertBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			vertBufferInfo.buffer = &_container.buffer;
			vertBufferInfo.bufferMemory = &_container.bufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, vertBufferInfo);

			VulkanUtilities::copyBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, stagingBuffer, _container.buffer, bufferSize);

			vkDestroyBuffer(myGraphicsCore.logicalDevice, stagingBuffer, nullptr);
			vkFreeMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, nullptr);

			return _container;
		}

		VulkanBufferContainer VulkanRBL::createIndexBuffer(Mesh mesh, DMKDataType type)
		{
			VulkanBufferContainer _container;
			_container.dataCount = mesh.indexes.size();

			VkDeviceSize bufferSize = _container.dataCount * (UI32)type;
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			VulkanCreateBufferInfo bufferInfo;
			bufferInfo.bufferSize = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			bufferInfo.buffer = &stagingBuffer;
			bufferInfo.bufferMemory = &stagingBufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, bufferInfo);

			void* data = nullptr;
			vkMapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, mesh.indexes.data(), (size_t)bufferSize);
			vkUnmapMemory(myGraphicsCore.logicalDevice, stagingBufferMemory);

			VulkanCreateBufferInfo indexBufferInfo;
			indexBufferInfo.bufferSize = bufferSize;
			indexBufferInfo.usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			indexBufferInfo.bufferMemoryPropertyflags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			indexBufferInfo.buffer = &_container.buffer;
			indexBufferInfo.bufferMemory = &_container.bufferMemory;
			VulkanUtilities::createBuffer(myGraphicsCore.logicalDevice, myGraphicsCore.physicalDevice, indexBufferInfo);

			VulkanUtilities::copyBuffer(myGraphicsCore.logicalDevice, myMainCommandBuffer.pool, myGraphicsCore.graphicsQueue, myGraphicsCore.presentQueue, stagingBuffer, _container.buffer, bufferSize);

			vkDestroyBuffer(myGraphicsCore.logicalDevice, stagingBuffer, nullptr);
			vkFreeMemory(myGraphicsCore.logicalDevice, stagingBufferMemory, nullptr);

			return _container;
		}

		VulkanTextureContainer VulkanRBL::createTextureImage(Texture texture)
		{
			VulkanUtilitiesTextureInitInfo initInfo;
			initInfo.logicalDevice = myGraphicsCore.logicalDevice;
			initInfo.physicalDevice = myGraphicsCore.physicalDevice;
			initInfo.commandPool = myMainCommandBuffer.pool;
			initInfo.processQueue = myGraphicsCore.graphicsQueue;
			initInfo.utilityQueue = myGraphicsCore.presentQueue;
			initInfo.mipLevels = 1;
			initInfo.minMipLevels = 0;
			initInfo.maxMipLevels = 1;
			initInfo.modeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			initInfo.modeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			initInfo.modeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			initInfo.magFilter = VK_FILTER_LINEAR;
			initInfo.minFilter = VK_FILTER_LINEAR;
			initInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			return VulkanUtilities::createTexture(initInfo, texture);
		}

		VulkanUnformBufferContainer VulkanRBL::createUniformBuffers(DMKUniformBufferObjectDescriptor uniformBufferDescriptor)
		{
			return VulkanUtilities::createUniformBuffers(
				myGraphicsCore.logicalDevice,
				myGraphicsCore.physicalDevice,
				DMKUniformBufferObjectDescriptor::uniformByteSize(uniformBufferDescriptor.attributes),
				mySwapChain.swapChainContainer.swapChainImages.size());
		}

		VulkanGraphicsDescriptor VulkanRBL::createDescriptors(ARRAY<DMKUniformBufferObjectDescriptor> descriptors, ARRAY<VulkanUnformBufferContainer> uniformBufferContainers, ARRAY<VulkanTextureContainer> textureContainers)
		{
			VulkanGraphicsDescriptor _descriptor;

			/* Initialize descriptor set layout */
			VulkanDescriptorSetLayoutInitInfo layoutInitInfo;
			layoutInitInfo.bindings = VulkanUtilities::getDescriptorSetBindings(descriptors);
			_descriptor.initializeLayout(myGraphicsCore.logicalDevice, layoutInitInfo);

			/* Initialize descriptor pool */
			VulkanDescriptorPoolInitInfo initInfo;
			initInfo.poolSizes = VulkanUtilities::getPoolSizes(descriptors, uniformBufferContainers.size(), textureContainers.size());
			_descriptor.initializePool(myGraphicsCore.logicalDevice, initInfo);

			/* Initialize descriptor sets */
			ARRAY<VkWriteDescriptorSet> _writes;

			for (UI32 binding = 0; binding < descriptors.size(); binding++)
			{
				ARRAY<VkDescriptorBufferInfo> bufferInfos;

				switch (descriptors[binding].type)
				{
					/* Initialize Uniform buffer descriptor */
				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT:
					for (UI32 itr = 0; itr < uniformBufferContainers.size(); itr++)
					{
						for (UI32 i = 0; i < uniformBufferContainers[itr].buffers.size(); i++)
						{
							VkDescriptorBufferInfo bufferInfo = {};
							bufferInfo.buffer = uniformBufferContainers[itr].buffers[i];
							bufferInfo.offset = 0;
							bufferInfo.range = DMKUniformBufferObjectDescriptor::uniformByteSize(descriptors[binding].attributes);
							bufferInfos.pushBack(bufferInfo);
						}
					}

					VkWriteDescriptorSet _writes1;
					_writes1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					_writes1.dstBinding = 0;
					_writes1.dstArrayElement = 0;
					_writes1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					_writes1.descriptorCount = 1;
					_writes1.pBufferInfo = bufferInfos.data();
					_writes1.pNext = VK_NULL_HANDLE;
					_writes1.pImageInfo = VK_NULL_HANDLE;
					_writes1.pTexelBufferView = VK_NULL_HANDLE;
					_writes.push_back(_writes1);
					break;

				case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT:
					break;

				default:
					for (UI32 _texIndex = 0; _texIndex < textureContainers.size(); _texIndex++)
					{
						VkDescriptorImageInfo imageInfo = {};
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = textureContainers[_texIndex].imageView;
						imageInfo.sampler = textureContainers[_texIndex].imageSampler;

						VkWriteDescriptorSet _writes2;
						_writes2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						_writes2.dstBinding = 1;
						_writes2.dstArrayElement = 0;
						_writes2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						_writes2.descriptorCount = 1;
						_writes2.pImageInfo = &imageInfo;
						_writes2.pNext = VK_NULL_HANDLE;
						_writes2.pTexelBufferView = VK_NULL_HANDLE;
						_writes2.pBufferInfo = VK_NULL_HANDLE;
						_writes.push_back(_writes2);
					}
					break;
				}
			}

			VulkanDescriptorSetsInitInfo setInitInfo;
			setInitInfo.descriptorWrites = _writes;
			_descriptor.initializeSets(myGraphicsCore.logicalDevice, setInitInfo);

			return _descriptor;
		}

		VulkanGraphicsPipeline VulkanRBL::createPipeline(ARRAY<VulkanGraphicsDescriptor> descriptors, ARRAY<DMKUniformBufferObjectDescriptor> uniformBufferDescriptors, ARRAY<DMKVertexAttribute> attributes, ShaderPaths paths, DMKObjectType objectType)
		{
			VulkanGraphicsPipeline _pipeline;

			/* Initialize pipeline layout */
			VulkanGraphicsPipelineLayoutInitInfo layoutInitInfo;
			for (auto _descriptor : descriptors)
				layoutInitInfo.layouts.pushBack(_descriptor.layout);

			UI32 _oldRangeSize = 0;
			for (auto _description : uniformBufferDescriptors)
			{
				if (_description.type != DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT)
					continue;

				VkPushConstantRange _range;
				_range.stageFlags = VulkanUtilities::getShaderStage(_description.location);
				_range.size = DMKUniformBufferObjectDescriptor::uniformByteSize(_description.attributes);
				_range.offset = _oldRangeSize;
				layoutInitInfo.pushConstantRanges.pushBack(_range);

				_oldRangeSize += _range.size;
			}
			_pipeline.initializePipelineLayout(myGraphicsCore.logicalDevice, layoutInitInfo);

			/* Initialize pipeline */
			auto _shaders = VulkanUtilities::getGraphicsShaders(myGraphicsCore.logicalDevice, paths);
			switch (objectType)
			{
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePresetSkyBox(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
				_pipeline.initializePipeline(
					myGraphicsCore.logicalDevice,
					VulkanPresets::pipelinePreset3D(
						myGraphicsCore.logicalDevice,
						myGraphicsCore.msaaSamples,
						myFrameBuffer.renderPass,
						_shaders,
						attributes,
						mySwapChain.swapChainContainer.swapChainExtent));
				break;
			case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
				break;
			}

			return _pipeline;
		}

		VulkanRenderData VulkanRBL::_initializeSkeletalAnimation(POINTER<InternalFormat> format)
		{
			//VulkanSkeletalAnimation* _renderObject = Cast<VulkanSkeletalAnimation*>(format.get());

			return VulkanRenderData();
		}
	}
}