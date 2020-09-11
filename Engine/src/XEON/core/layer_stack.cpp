#include "xeonpch.h"
#include "XEON/core/layer_stack.h"

namespace XEON {
	
	LayerStack::LayerStack() : layers(std::vector<Layer*>()) { }

	LayerStack::~LayerStack() {
		for (Layer* layer : layers) {
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer) {
		layers.emplace(layers.begin() + layerInsert, layer);
		layerInsert++;
	}

	void LayerStack::pushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
	}

	bool LayerStack::popLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			layerInsert--;
			return true;
		}
		return false;
	}

	bool LayerStack::popOverlay(Layer* overlay) {
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it != layers.end()) {
			layers.erase(it);
			return true;
		}
		return false;
	}

}
