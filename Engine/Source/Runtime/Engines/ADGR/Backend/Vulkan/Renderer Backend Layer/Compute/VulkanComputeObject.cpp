#include "dmkafx.h"
#include "VulkanComputeObject.h"

#include "../Common/VulkanUtilities.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			void VulkanComputeObject::initialize()
			{
			}

			void VulkanComputeObject::initialize(ADGRVulkanComputeInputContainer container)
			{
			}

			void VulkanComputeObject::initialize(ADGRVulkanComputeBufferInputContainer container)
			{
			}

			void VulkanComputeObject::initialize(ADGRVulkanComputeImageInputContainer container)
			{
			}
			
			void VulkanComputeObject::terminate()
			{
				myComputeData.computeDescriptor; // terminate
			}
		}
	}
}