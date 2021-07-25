#include "gepch.h"

#include "LayerStack.h"

namespace GooE {

	LayerStack::~LayerStack() {
		for (Layer* layer : layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layer->OnDetach();
			layers.erase(it);
			layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it != layers.end()) {
			overlay->OnDetach();
			layers.erase(it);
		}
	}
}
