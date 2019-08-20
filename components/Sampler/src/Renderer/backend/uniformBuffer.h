#pragma once

#include "core/Buffer.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class uniformBuffer : public Buffer {
			public:
				uniformBuffer() {}
				~uniformBuffer() {}

				void init() override;
				void createDescriptorSetLayout() override;
			};
		}
	}
}
