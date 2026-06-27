#pragma once
#include "Hazle/Core/Hazle.h"

class Sandbox2D : public Hazle::Layer
{
public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazle::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazle::Event& e) override; 

private:
	////////////////////TEMPORARY////////////////////////
	Hazle::Ref<Hazle::Shader> m_Shader;
	Hazle::OrthographicCameraController m_CameraController;
	Hazle::Ref<Hazle::Texture2D> m_Texture;
	Hazle::Ref<Hazle::VertexArray> m_SquareVA;	
	Hazle::Ref<Hazle::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};