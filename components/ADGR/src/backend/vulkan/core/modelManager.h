#pragma once

#include "core/Model.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class ADGR_API modelManager : public Model {
			public:
				modelManager() {}
				~modelManager() {}

				void loadModel(DMKModelLoadInfo info) override;
			};
		}
	}
}
