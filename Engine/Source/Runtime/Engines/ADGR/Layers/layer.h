#pragma once

#ifndef _DYNAMIK_ADGR_LAYER_H
#define _DYNAMIK_ADGR_LAYER_H

#include "Events/DMKEventComponent.h"
#include "core.h"

namespace Dynamik {
	namespace ADGR {
		class DMK_API Layer {
		public:
			Layer(const std::string& name = "Layer 1");
			virtual ~Layer();

			virtual void attach() {}
			virtual void detach() {}
			virtual void update() {}
			virtual void onEvent(DMKEventComponent& event) {}

			inline const std::string& getName() const { return debugName; }

		protected:
			std::string debugName = "";
		};
	}
}

#endif // !_DYNAMIK_ADGR_LAYER_H
