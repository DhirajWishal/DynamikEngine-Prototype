#pragma once

#include "layer.h"

namespace Dynamik {
	namespace ADGR {

		class ADGR_API layerStack {
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
			std::vector<Layer*> layers;
			std::vector<Layer*>::iterator layerInsert;
		};
	}
}