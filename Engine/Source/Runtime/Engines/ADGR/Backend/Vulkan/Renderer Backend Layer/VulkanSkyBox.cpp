#include "dmkafx.h"
#include "VulkanSkyBox.h"

#include "Common/VulkanUtilities.h"

#include "Objects/InternalFormat/InternalFormat.h"

#include <stb_image.h>

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanSkyBox::VulkanSkyBox(ADGRVulkanGraphicsRenderableObjectInitInfo info)
				: VulkanGraphicsRenderableObject(info)
			{
				myRenderData.type = DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;
			}

			ADGRVulkanRenderData VulkanSkyBox::initializeObject(POINTER<InternalFormat> _object, VkSampleCountFlagBits msaaSamples)
			{
				ADGRVulkanDescriptorSetLayoutInitInfo layoutInitInfo;
				layoutInitInfo.bindings = VulkanUtilities::getDescriptorSetBindings(myInternalFormat->descriptor.uniformBufferObjectDescriptions);
				myRenderData.descriptors.initializeLayout(logicalDevice, layoutInitInfo);

				ADGRVulkanGraphicsPipelineLayoutInitInfo pipelineLayoutInitInfo;
				pipelineLayoutInitInfo.layouts = { myRenderData.descriptors.layout };
				initializePipelineLayout(pipelineLayoutInitInfo);

				ARRAY<VulkanGraphicsShader> _shaders = VulkanUtilities::getGraphicsShaders(logicalDevice, _object);

				// initialize pipeline
				ADGRVulkanGraphicsPipelineInitInfo pipelineInitInfo;
				pipelineInitInfo.shaders = _shaders;
				pipelineInitInfo.multisamplerMsaaSamples = msaaSamples;
				pipelineInitInfo.vertexBindingDescription = VulkanUtilities::getBindingDescription(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes, 1);
				pipelineInitInfo.vertexAttributeDescription = VulkanUtilities::getAttributeDescriptions(myInternalFormat->descriptor.vertexBufferObjectDescription.attributes);
				pipelineInitInfo.isTexturesAvailable = _object->texturePaths.size();
				pipelineInitInfo.rasterizerFrontFace = VK_FRONT_FACE_CLOCKWISE;
				initializePipeline(pipelineInitInfo);

				VulkanUtilities::terminateGraphicsShaders(logicalDevice, _shaders);

				// initialize textures
				initializeObjectResources();

				// initialize uniform buffers
				initializeUniformBuffer();

				// initialize descriptor pool
				initializeDescriptorPool();

				// initialize descriptor sets
				initializeDescriptorSets();

				/*
				ADGRVulkanGraphicsSupportObjectInitInfo supportObjectInitInfo;
				supportObjectInitInfo.logicalDevice = logicalDevice;
				supportObjectInitInfo.physicalDevice = physicalDevice;
				supportObjectInitInfo.commandPool = commandPool;
				supportObjectInitInfo.graphicsQueue = graphicsQueue;
				supportObjectInitInfo.presentQueue = presentQueue;
				myBRDF = VulkanBRDF(supportObjectInitInfo);
				myBRDF.initialize();

				myIrradianceCube = VulkanIrradianceCube(supportObjectInitInfo);
				myIrradianceCube.skyboxRenderData = myRenderData;
				myIrradianceCube.initialize();

				myPreFilteredCube = VulkanPrefilteredCube(supportObjectInitInfo);
				myPreFilteredCube.skyboxRenderData = myRenderData;
				myPreFilteredCube.initialize();
				*/

				return myRenderData;
			}

			void VulkanSkyBox::initializeUniformBuffer()
			{
				myRenderData.uniformBufferContainers.pushBack(
					VulkanUtilities::createUniformBuffers(
						logicalDevice,
						physicalDevice,
						sizeof(UniformBufferObject),
						myRenderData.swapChainPointer->swapChainImages.size()
						)
					);
			}

			void VulkanSkyBox::updateUniformBuffer(UniformBufferObject uniformBufferObject, UI32 currentImage)
			{
				for (ADGRVulkanUnformBufferContainer _container : myRenderData.uniformBufferContainers)
				{
					void* data = nullptr;
					vkMapMemory(logicalDevice, _container.bufferMemories[currentImage], 0, sizeof(uniformBufferObject), 0, &data);
					memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
					vkUnmapMemory(logicalDevice, _container.bufferMemories[currentImage]);
				}
			}
		}
	}
}