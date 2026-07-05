#pragma once
#include "Hazle/Core/Hazle.h"
#include "ParticleSystem.h"


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
	float m_SquareRotation = 0.0f;
	Hazle::OrthographicCameraController m_CameraController;
	Hazle::Ref<Hazle::Texture2D> m_Texture;
	Hazle::Ref<Hazle::VertexArray> m_QuadVA;	
	Hazle::Ref<Hazle::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_QuadColor = {0.2f, 0.3f, 0.8f, 1.0f};

	struct ProfileResult
	{
		const char* name;
		float time;
	};
	std::vector<ProfileResult> m_ProfileResults;

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
};