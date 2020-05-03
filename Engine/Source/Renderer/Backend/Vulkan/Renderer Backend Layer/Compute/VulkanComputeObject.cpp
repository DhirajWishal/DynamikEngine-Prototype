#include "dmkafx.h"
#include "VulkanComputeObject.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace Renderer {
		namespace Backend {
			void VulkanComputeObject::initialize()
			{
			}

			void VulkanComputeObject::initialize(VulkanComputeInputContainer container)
			{
			}

			void VulkanComputeObject::initialize(VulkanComputeBufferInputContainer container)
			{
			}

			void VulkanComputeObject::initialize(VulkanComputeImageInputContainer container)
			{
			}
			
			void VulkanComputeObject::terminate()
			{
				myComputeData.computeDescriptor; // terminate
			}
		}
	}
}