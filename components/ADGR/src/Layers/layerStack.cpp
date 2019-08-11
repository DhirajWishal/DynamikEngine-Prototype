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
			layerInsert = layers.emplace(layerInsert, layer);
		}

		void layerStack::pushOverLay(Layer* overlay) {
			layers.emplace_back(overlay);
		}

		void layerStack::popLayer(Layer* layer) {
			auto it = std::find(layers.begin(), layers.end(), layer);

			if (it != layers.end()) {
				layers.erase(it);
				layerInsert--;
			}
		}

		void layerStack::popOverLay(Layer* overlay) {
			auto it = std::find(layers.begin(), layers.end(), overlay);

			if (it != layers.end())
				layers.erase(it);
		}
	}
}