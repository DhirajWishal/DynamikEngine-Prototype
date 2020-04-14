#include "adgrafx.h"
#include "VulkanComputeManager.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeManager::initialize()
			{
				myComputeCore.initializeInstance();
				myComputeCore.initializeDevice();
				myComputeCommandBuffer.initializeCommandPool(myComputeCore.logicalDevice, myComputeCore.queueFamilyIndex);

				ADGRVulkanComputeObjectInitInfo objectInitInfo;
				objectInitInfo.logicalDevice = myComputeCore.logicalDevice;
				objectInitInfo.physicalDevice = myComputeCore.physicalDevice;
				objectInitInfo.physicalDeviceMemoryProperties = myComputeCore.physicalDeviceMemoryProperties;
				objectInitInfo.queueFamilyIndex = myComputeCore.queueFamilyIndex;
				VulkanComputeObject _object(objectInitInfo);
				_object.initialize();
				myComputeDatas.pushBack(_object.myComputeData);

				ADGRVulkanComputeCommandBufferInitInfo commandBufferInitInfo;
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