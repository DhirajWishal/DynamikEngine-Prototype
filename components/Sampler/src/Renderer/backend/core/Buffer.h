#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class Buffer : public Core {
			public:
				Buffer() {}
				virtual ~Buffer() {}

			protected:
				virtual void createDescriptorSetLayout() {}
			};
		}
	}
}