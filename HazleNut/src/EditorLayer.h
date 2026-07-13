#pragma once
#include "Hazle/Core/Hazle.h"

namespace Hazle
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		private:
			float m_SquareRotation = 0.0f;
			OrthographicCameraController m_CameraController;
			Ref<Shader> m_Shader;
			Ref<Texture2D> m_Texture;
			Ref<VertexArray> m_QuadVA;
			Ref<Texture2D> m_CheckerboardTexture;
			Ref<Texture2D> m_SpriteSheet;
			Ref<SubTexture2D> m_SubTexture;
			Ref<FrameBuffer> m_FrameBuffer;
			Ref<Scene> m_ActiveScene;
			Entity m_SquareEntity;
			Entity m_CameraEntity, m_SecondCamera;
			Entity m_PrimaryCameraptr;
			bool m_PrimaryCamera = true;

			glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
			glm::vec4 m_QuadColor = { 0.2f, 0.3f, 0.8f, 1.0f };

			struct ProfileResult
			{
				const char* name;
				float time;
			};
			std::vector<ProfileResult> m_ProfileResults;

			std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;
			uint32_t m_MapWidth, m_MapHeight;

			bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}