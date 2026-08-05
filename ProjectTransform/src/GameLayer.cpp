#include "GameLayer.h"

namespace Transform
{
	GameLayer::GameLayer()
		: Layer("GameLayer")
	{}

	GameLayer::~GameLayer()
	{}
	
	void GameLayer::OnAttach()
	{}
	
	void GameLayer::OnDetach()
	{}
	
	void GameLayer::OnUpdate(Hazle::Timestep ts)
	{}
	
	void GameLayer::OnImGuiRender()
	{}
	
	void GameLayer::OnEvent(Hazle::Event& event)
	{}
}