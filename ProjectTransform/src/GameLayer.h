#pragma once
#include "Hazle/Core/Hazle.h"

namespace Transform
{
	class GameLayer : public Hazle::Layer
	{
	public:
		GameLayer();
		~GameLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Hazle::Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Hazle::Event& event) override;


	private:
		Hazle::Entity m_Player;
	};
}