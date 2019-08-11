#pragma once

#include "event.h"
#include "backend/interface.h"

namespace Dynamik {
	namespace ADGR {

		class ADGR_API Layer {
		public:
			Layer(const std::string& name = "Layer 1");
			virtual ~Layer();

			virtual void attach() {}
			virtual void detach() {}
			virtual void update() {}
			virtual void onEvent(Event& event) {}

			inline const std::string& getName() const { return debugName; }

		protected:
			std::string debugName;
		};
	}
}
