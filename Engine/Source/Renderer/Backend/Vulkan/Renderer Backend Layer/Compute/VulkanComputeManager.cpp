#include "dmkafx.h"
#include "VulkanComputeManager.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanComputeManager::initialize()
			{
				myComputeCore.initializeInstance();
				myComputeCore.initializeDevice();
				myComputeCommandBuffer.initializeCommandPool(myComputeCore.logicalDevice, myComputeCore.queueFamilyIndices.computeFamily.value());

				VulkanComputeObjectInitInfo objectInitInfo;
				objectInitInfo.logicalDevice = myComputeCore.logicalDevice;
				objectInitInfo.physicalDevice = myComputeCore.physicalDevice;
				objectInitInfo.physicalDeviceMemoryProperties = myComputeCore.physicalDeviceMemoryProperties;
				objectInitInfo.queueFamilyIndex = myComputeCore.queueFamilyIndices.computeFamily.value();
				VulkanComputeObject _object(objectInitInfo);
				_object.initialize();
				myComputeDatas.push_back(_object.myComputeData);

				VulkanComputeCommandBufferInitInfo commandBufferInitInfo;
				commandBufferInitInfo.computeDatas = myComputeDatas;
				myComputeCommandBuffer.initializeCommandBuffers(myComputeCore.logicalDevice, commandBufferInitInfo);
			}

			void VulkanComputeManager::compute()
			{
				myComputeCore.submitQueue(myComputeCommandBuffer.buffers);
				myComputeCore.finishCompute();
			}

			void VulkanComputeManager::terminate()
			{
			}
		}
	}
}