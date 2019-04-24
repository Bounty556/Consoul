#pragma once

#include "Layer.h"
#include "Window.h"

namespace Soul {
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		void Update(float deltaTime);
		void Draw(const Window* target);

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerPlacement;
	};
}