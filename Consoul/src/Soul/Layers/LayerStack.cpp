#include "pch.h"
#include "LayerStack.h"

namespace Soul {
	std::vector<Layer*> LayerStack::m_Layers;
	std::vector<Layer*> LayerStack::m_Overlays;
	std::vector<std::pair<LayerStack::Action, Layer*>> LayerStack::m_Queue;

	void LayerStack::QueueAction(Action action, Layer* layer)
	{
		m_Queue.push_back(std::pair<Action, Layer*>(action, layer));
	}

	bool LayerStack::AllEmpty()
	{
		return m_Layers.empty() && m_Overlays.empty() && m_Queue.empty();
	}

	void LayerStack::CleanUp()
	{
		for (Layer* layer : m_Layers)
			delete layer;

		for (Layer* overlay : m_Overlays)
			delete overlay;

		m_Layers.clear();
		m_Overlays.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto found = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (found != m_Layers.end())
			m_Layers.erase(found);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Overlays.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto found = std::find(m_Overlays.begin(), m_Overlays.end(), overlay);

		if (found != m_Overlays.end())
			m_Overlays.erase(found);
	}

	void LayerStack::Update(double deltaTime)
	{
		for (auto it = m_Queue.rbegin(); it != m_Queue.rend(); ++it)
		{
			switch ((*it).first)
			{
			case Action::QueuePushLayer:
				PushLayer((*it).second);
				break;
			case Action::QueuePushOverlay:
				PushOverlay((*it).second);
				break;
			case Action::QueuePopLayer:
				PopLayer((*it).second);
				break;
			case Action::QueuePopOverlay:
				PopOverlay((*it).second);
				break;
			case Action::QueueFullClear:
				CleanUp();
				break;
			}
		}
		m_Queue.clear();

		for (auto it = m_Overlays.begin(); it != m_Overlays.end(); ++it)
			if (!(*it)->Update(deltaTime)) return;

		for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it)
			if (!(*it)->Update(deltaTime)) return;
	}

	void LayerStack::Draw(const Window* target)
	{
		for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it)
		{
			if ((*it)->IsDrawn)
				target->DrawToBuffer((*it)->Draw(), (*it)->GetLength(),
				(*it)->GetXPos(), (*it)->GetYPos());
		}

		for (auto it = m_Overlays.begin(); it != m_Overlays.end(); ++it)
		{
			if ((*it)->IsDrawn)
				target->DrawToBuffer((*it)->Draw(), (*it)->GetLength(),
				(*it)->GetXPos(), (*it)->GetYPos());
		}
	}
}