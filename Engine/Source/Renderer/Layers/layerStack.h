#pragma once

#ifndef _DYNAMIK_RENDERER_LAYER_STACK_H
#define _DYNAMIK_RENDERER_LAYER_STACK_H

#include "layer.h"

namespace Dynamik {
	namespace Renderer {
		class layerStack {
		public:
			layerStack();
			~layerStack();

			void pushLayer(Layer* layer);
			void pushOverLay(Layer* overLay);
			void popLayer(Layer* layer);
			void popOverLay(Layer* overLay);

			std::vector<Layer*>::iterator begin() { return layers.begin(); }
			std::vector<Layer*>::iterator end() { return layers.end(); }

		private:
			std::vector<Layer*> layers = {};
			std::vector<Layer*>::iterator layerInsert = {};
		};
	}
}
#endif // !_DYNAMIK_RENDERER_LAYER_STACK_H
