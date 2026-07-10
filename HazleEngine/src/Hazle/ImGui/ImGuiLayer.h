#pragma once

#include "Hazle/Core/Layer.h"
#include "Hazle/Event/ApplicationEvent.h"
#include "Hazle/Event/KeyEvent.h"
#include "Hazle/Event/MouseEvent.h"

namespace Hazle {

	class HAZLE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		
		void SetBlockEvents(bool Block) { m_blockEvents = Block; };
		void begin();
		void end();
	private:
		bool m_blockEvents = false;
		float m_Time = 0.0f;
	};

}