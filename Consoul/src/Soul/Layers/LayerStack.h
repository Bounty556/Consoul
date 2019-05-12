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
			QueuePopOverlay,
			QueueFullClear
		};
	public:
		LayerStack() = delete;

		/*
		Adds a new action to the queue, along with the associated layer the action should act upon.

		Action action: Action to queue
		Layer* layer: Layer to be acted on
		*/
		static void QueueAction(Action action, Layer* layer);

		/*
		Clears the queue by calling every action in it.
		Sends an update call to all the layers in the stack.

		double deltaTime: The length of the previous frame in seconds
		*/
		static void Update(double deltaTime);

		/*
		Gets all char* representations of each layer and draws them to the given window

		const Window* target: The window to draw to
		*/
		static void Draw(const Window* target);

		/*
		Returns: true if there are no layers, overlays, or anything in the queue. False otherwise
		*/
		static bool AllEmpty();

		/*
		Cleans up allocated memory
		*/
		static void CleanUp();
	private:
		static void PushLayer(Layer* layer);
		static void PopLayer(Layer* layer);
		static void PushOverlay(Layer* overlay);
		static void PopOverlay(Layer* overlay);
	private:
		static std::vector<Layer*> m_Layers;
		static std::vector<Layer*> m_Overlays;
		static std::vector<std::pair<Action, Layer*>> m_Queue;
	};
}