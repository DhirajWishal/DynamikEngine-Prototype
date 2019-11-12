#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Window : public Core {
			public:
				Window() {}
				virtual ~Window() {}

				virtual void pollEvents() {}
				virtual void onUpdate() {}
				virtual bool closeEvent(ADGRVulkanDataContainer* container) { return false; }
			};
		}
	}
}
