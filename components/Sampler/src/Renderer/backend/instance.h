#pragma once

#include "core/Instance.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class instance : public Instance {
			public:
				instance() {}
				~instance() {}

				void init() override;

			private:
				VkInstance* m_instance = myInstance;
			};
		}
	}
}
