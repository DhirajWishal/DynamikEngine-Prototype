#pragma once

#include "core/Instance.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class instanceManager : public Instance {
			public:
				instanceManager() {}
				~instanceManager() {}

				void init(ADGRVulkanDataContainer* container) override;

				void clear(ADGRVulkanDataContainer* container) override;

				VkInstance* getInstanceAddr() { return m_instance; }
				VkSurfaceKHR getSurface() { return surface; }

			private:
				VkInstance* m_instance = &instance;
			};
		}
	}
}
