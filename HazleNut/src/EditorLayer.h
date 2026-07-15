#pragma once
#include "Hazle/Core/Hazle.h"
#include "Panels/SceneHierarchyPanel.h"

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
			OrthographicCameraController m_CameraController;
			Ref<FrameBuffer> m_FrameBuffer;
			Ref<Scene> m_ActiveScene;
			Entity m_SquareEntity;
			Entity m_CameraEntity;

			glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
			struct ProfileResult
			{
				const char* name;
				float time;
			};
			std::vector<ProfileResult> m_ProfileResults;

			std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;
			uint32_t m_MapWidth, m_MapHeight;

			bool m_ViewportFocused = false, m_ViewportHovered = false;

			//Panels
			SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}