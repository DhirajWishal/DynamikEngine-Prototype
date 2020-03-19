#include "adgrafx.h"
#include "layerStack.h"

namespace Dynamik {
	namespace ADGR {
		layerStack::layerStack() {
			layerInsert = layers.begin();
		}

		layerStack::~layerStack() {
			for (auto* layer : layers)
				delete layer;
		}

		void layerStack::pushLayer(Layer* layer) {
			layers.emplace(layerInsert, (POINTER<Layer>)layer);
		}

		void layerStack::pushOverLay(Layer* overlay) {
			layers.pushBack(overlay);
		}

		void layerStack::popLayer(Layer* layer) {
			auto it = std::find(layers.begin(), layers.end(), layer);

			if (it != layers.end()) {
				layers.remove(it);
				layerInsert--;
			}
		}

		void layerStack::popOverLay(Layer* overlay) {
			auto it = std::find(layers.begin(), layers.end(), overlay);

			if (it != layers.end())
				layers.remove(it);
		}
	}
}