#pragma once

#include "XEON/core/core.h"
#include "XEON/core/layer.h"

#include <vector>

namespace XEON {

	class XEON_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		bool popLayer(Layer* layer);
		bool popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
	private:
		std::vector<Layer*> layers;
		size_t layerInsert = 0;
	};

}
