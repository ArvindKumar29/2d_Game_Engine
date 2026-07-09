#pragma once
#include "Hazle/Core/Hazle.h"

namespace Hazle
{
	class EditorLayer : public Hazle::Layer
	{
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		private:
			Hazle::Ref<Hazle::Shader> m_Shader;
			float m_SquareRotation = 0.0f;
			Hazle::OrthographicCameraController m_CameraController;
			Hazle::Ref<Hazle::Texture2D> m_Texture;
			Hazle::Ref<Hazle::VertexArray> m_QuadVA;
			Hazle::Ref<Hazle::Texture2D> m_CheckerboardTexture;
			Hazle::Ref<Hazle::Texture2D> m_SpriteSheet;
			Hazle::Ref<Hazle::SubTexture2D> m_SubTexture;
			Hazle::Ref<Hazle::FrameBuffer> m_FrameBuffer;

			glm::vec4 m_QuadColor = { 0.2f, 0.3f, 0.8f, 1.0f };

			struct ProfileResult
			{
				const char* name;
				float time;
			};
			std::vector<ProfileResult> m_ProfileResults;

			std::unordered_map<char, Hazle::Ref<Hazle::SubTexture2D>> m_TextureMap;
			uint32_t m_MapWidth, m_MapHeight;
	};
}