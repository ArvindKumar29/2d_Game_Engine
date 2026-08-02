#pragma once

#include "core.h"
#include "Layer.h"

#include <vector>

namespace Hazle
{
	class HAZLE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer); // Pushes a layer onto the stack at m_LayerInsertIndex and increments m_LayerInsertIndex
		void PushOverlay(Layer* overlay); // Pushes an overlay onto the stack at the end of the stack
		void PopLayer(Layer* layer); // Pops a layer from the stack and decrements
		void PopOverlay(Layer* overlay); // Pops an overlay from the stack

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); } // Returns an iterator to the beginning of the layer stack
		std::vector<Layer*>::iterator end() { return m_Layers.end(); } // Returns an iterator to the end of the layer stack
					
	protected:		
		std::vector<Layer*> m_Layers; // The layer stack contains a vector of Layer(both layer and overlays) pointers and helps keep the code cache friendly
		unsigned int m_LayerInsertIndex = 0; // The index at which the next layer will be inserted. Overlays are always added to the end of the stack, so this index is only used for layers.
	};
}