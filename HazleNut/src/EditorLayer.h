#pragma once
#include "Hazle/Core/Hazle.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
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

		void OnDuplicateEntity();

		//UI Panels
		void UI_Toolbar();
		void OnScenePlay();
		void OnSceneStop();

	private:
		bool OnKeyPresedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPresedEvent(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();

		std::string currentPath;

		OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene, m_RuntimeScene;
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
		ContentBrowserPanel m_ContentBrowserPanel;


		enum class SceneState
		{
			Edit,
			Play
		};

		SceneState m_SceneState = SceneState::Edit;
		enum class EditorTheme
		{
			Dark,
			Light
		};
		EditorTheme m_EditorTheme = EditorTheme::Dark;

		// HazleUI Icons
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSun, m_IconMoon, m_IconSave;
		
	};
}