#include "pch.h"
#include "LayerStack.h"

namespace Soul {
	LayerStack::LayerStack()
		: m_LayerPlacement(m_Layers.begin())
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;

		m_Layers.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerPlacement = m_Layers.emplace(m_LayerPlacement, layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto found = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (found != m_Layers.end())
		{
			m_Layers.erase(found);
			--m_LayerPlacement;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto found = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (found != m_Layers.end())
			m_Layers.erase(found);
	}

	void LayerStack::Update(float deltaTime)
	{
		for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it)
			(*it)->Update(deltaTime);
	}

	void LayerStack::Draw(const Window* target)
	{
		for (auto it = m_Layers.begin(); it != m_Layers.end(); ++it)
		{
			if ((*it)->IsDrawn)
				target->DrawToBuffer((*it)->Draw(), (*it)->GetLength(), (*it)->GetXPos(), (*it)->GetYPos());
		}
	}
}