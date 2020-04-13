#include "adgrafx.h"
#include "VulkanSwapChain3D.h"

#include "Objects/VulkanFunctions.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanSwapChain3D::basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCapabilities)
			{
				myPhysicalDevice = physicalDevice;
				swapChainContainer.setBasicData(device, physicalDevice, surface, surfaceCapabilities);
			}

			void VulkanSwapChain3D::initializeSwapChain(UI32 width, UI32 height)
			{
				swapChainContainer.initializeSwapChain(width, height);
			}

			void VulkanSwapChain3D::terminate()
			{
				swapChainContainer.terminate();
			}
		}
	}
}