#include "adgrafx.h"
#include "VulkanOneTimeCompute.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			VulkanOneTimeCompute::VulkanOneTimeCompute()
			{
				VulkanComputeManager _manager;
				_manager.initialize();
				_manager.compute();
				_manager.terminate();
			}
			
			VulkanOneTimeCompute::~VulkanOneTimeCompute()
			{
			}
		}
	}
}