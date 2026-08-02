#include <hzpch.h>
#include "LayerStack.h"

namespace Hazle
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
	}

	void LayerStack::PushLayer(Layer* layer) // Adds a layer to the stack at the position of m_LayerInsertIndex and increments m_LayerInsertIndex
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) // Adds an overlay to the end of the stack to seperate it from the layers and does not increment m_LayerInsertIndex
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) // Removes a layer from the stack and decrements m_LayerInsertIndex
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) // Removes an overlay from the stack and does not decrement m_LayerInsertIndex
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}