#include "dmkafx.h"
#include "VulkanGraphicsSwapChain3D.h"

#include "Graphics/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanGraphicsSwapChain3D::basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities)
			{
				myPhysicalDevice = physicalDevice;
				swapChainContainer.setBasicData(device, physicalDevice, surface, surfaceCapabilities);
			}

			void VulkanGraphicsSwapChain3D::initializeSwapChain(UI32 width, UI32 height)
			{
				swapChainContainer.initializeSwapChain(width, height);
			}

			void VulkanGraphicsSwapChain3D::terminate()
			{
				swapChainContainer.terminate();
			}
		}
	}
}