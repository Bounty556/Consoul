#pragma once

#include "Layer.h"
#include "Window.h"

namespace Soul {
	class LayerStack
	{
	public:
		enum Action : unsigned char
		{
			QueuePushLayer,
			QueuePushOverlay,
			QueuePopLayer,
			QueuePopOverlay
		};
	public:
		LayerStack();
		~LayerStack();

		void QueueAction(Action action, Layer* layer);

		void Update(double deltaTime);
		void Draw(const Window* target);
	private:
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_Overlays;
		std::vector<std::pair<Action, Layer*>> m_Queue;
	};
}