#pragma once

#ifndef _DYNAMIK_RENDERER_LAYER_H
#define _DYNAMIK_RENDERER_LAYER_H

#include "Events/EventComponent.h"


namespace Dynamik {
	namespace Renderer {
		class Layer {
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

#endif // !_DYNAMIK_RENDERER_LAYER_H
