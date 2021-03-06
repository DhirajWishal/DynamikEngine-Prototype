#include "dmkafx.h"
#include "layerStack.h"

namespace Dynamik {
	namespace Renderer {
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
			layers.push_back(overlay);
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