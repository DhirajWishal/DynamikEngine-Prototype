#include "dmkafx.h"
#include "VulkanGraphicsSwapChain3D.h"

#include "Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanGraphicsSwapChain3D::basicInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VulkanSurfaceContainer container)
			{
				myPhysicalDevice = physicalDevice;
				swapChainContainer.setBasicData(device, physicalDevice, container);
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