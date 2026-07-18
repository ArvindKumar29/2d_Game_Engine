#pragma once
#include "Hazle/Core/Hazle.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Hazle/Renderer/EditorCamera.h"

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
		bool OnKeyPresedEvent(KeyPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		void SaveScene();

		std::string currentPath;

		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity, m_SecondCamera;

		EditorCamera m_EditorCamera;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
		glm::vec2 m_ViewportBounds[2];

		struct ProfileResult
		{
			const char* name;
			float time;
		};
		std::vector<ProfileResult> m_ProfileResults;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;
		uint32_t m_MapWidth, m_MapHeight;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = -1;
		Entity m_HoveredEntity;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}