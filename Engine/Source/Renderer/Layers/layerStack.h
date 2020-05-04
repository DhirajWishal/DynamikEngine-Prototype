#pragma once

#ifndef _DYNAMIK_RENDERER_LAYER_STACK_H
#define _DYNAMIK_RENDERER_LAYER_STACK_H

#include "layer.h"

namespace Dynamik {
	namespace Renderer {
		class  layerStack {
		public:
			layerStack();
			~layerStack();

			void pushLayer(Layer* layer);
			void pushOverLay(Layer* overLay);
			void popLayer(Layer* layer);
			void popOverLay(Layer* overLay);

			ARRAY<Layer*>::iterator begin() { return layers.begin(); }
			ARRAY<Layer*>::iterator end() { return layers.end(); }

		private:
			ARRAY<Layer*> layers = {};
			ARRAY<Layer*>::iterator layerInsert = {};
		};
	}
}
#endif // !_DYNAMIK_RENDERER_LAYER_STACK_H
