#include "xeonpch.h"
#include "XEON/layer_stack.h"

namespace XEON {
	
	LayerStack::LayerStack() : layers(std::vector<Layer*>()) { }

	LayerStack::~LayerStack() {
		for (Layer* layer : layers) delete layer;
	}

	void LayerStack::pushLayer(Layer* layer) {
		layers.emplace(layers.begin() + layerInsert, layer);
		layerInsert++;
		layer->onAttach();
	}

	void LayerStack::pushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
		overlay->onAttach();
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			layerInsert--;
			layer->onDetach();
		}
	}

	void LayerStack::popOverlay(Layer* overlay) {
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it != layers.end()) {
			layers.erase(it);
			overlay->onDetach();
		}
	}

}
