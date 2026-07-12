#include <hzpch.h>
#include "EditorLayer.h"

namespace Hazle
{
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"),
		m_CameraController(1280.0f, 720.0f, true)
	{}


	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = Texture2D::Create("Assets/Textures/checkerboard.png");
		m_SpriteSheet = Texture2D::Create("Assets/RPG_base_assets/kenney_rpg-base/Spritesheet/RPGpack_sheet_2X.png");
		m_SubTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128, 128 }, { 1, 1 });


		/*m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
		m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
		m_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
		

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };*/

		m_CameraController.SetZoomLevel(10.0f);

		FrameBufferSpecifications fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		Entity SquareEntity = m_ActiveScene->CreateEntity("Square");

		SquareEntity.AddComponent<CTransform>(glm::mat4(1.0f));
		SquareEntity.AddComponent<CSpriteRenderer>(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });

		if (SquareEntity.hasComponent<CTransform>())
			HZ_CORE_INFO("Squar Entity has Transform");
		else
			HZ_CORE_INFO("Squar Entity has no Transform component");
		
		m_SquareEntity = SquareEntity;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CCamera>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		m_CameraEntity.AddComponent<CTransform>();
		
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera");
		m_SecondCamera.AddComponent<CCamera>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		m_SecondCamera.AddComponent<CTransform>();
		m_SecondCamera.getComponent<CCamera>().Primary = false;
	}

	void EditorLayer::OnDetach()
	{

	}


	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		// Update
		m_FrameBuffer->Bind();
		
		if (m_ViewportFocused)
		{
			HZ_PROFILE_SCOPE("EditorLayer::OnUpdate");
			m_CameraController.OnUpdate(ts);
		}
		
		if (Input::IsKeyPressed(Key::Escape))
		{
			Application::Get().Close();
		}

		// Render
		m_SquareRotation += ts * glm::radians(360.0f);

		Renderer2D::ResetStats();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();
		// UPDATE SCENE
		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Renderer2D::DrawQuad({ 1.0f, 2.0f, 1.0f }, 0.0f, { 2.0f, 2.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
		//Renderer2D::EndScene();
		m_ActiveScene->OnUpdate(ts);
		m_FrameBuffer->Unbind();
	}


	void EditorLayer::OnImGuiRender()
	{
		static bool dockSpaceOpen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockSpaceOpen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (dockSpaceOpen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.QuadCount * 4);
		ImGui::Text("Indices: %d", stats.QuadCount * 6);

		if (m_SquareEntity.hasComponent<CTag>())
		{
			ImGui::Separator();
			auto tag = m_SquareEntity.getComponent<CTag>().Tag;
			ImGui::Text("%s", tag.c_str());
			auto& squareColor = m_SquareEntity.getComponent<CSpriteRenderer>().Color;
			ImGui::ColorEdit4("Quad Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Trasnform: ",
			glm::value_ptr(m_CameraEntity.getComponent<CTransform>().Transform[3]));

		if (ImGui::Checkbox("Camera A: ", &m_PrimaryCamera))
		{
			m_CameraEntity.getComponent<CCamera>().Primary = m_PrimaryCamera;
			m_SecondCamera.getComponent<CCamera>().Primary = !m_PrimaryCamera;
		}

		m_ProfileResults.clear();
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0.0f)
		{
			m_FrameBuffer->Resize(uint32_t(viewportPanelSize.x), uint32_t(viewportPanelSize.y));
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		HZ_CORE_WARN("Focoused: {0}", m_ViewportFocused);
		HZ_CORE_WARN("Hovered: {0}", m_ViewportHovered);
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("File Explorer");
		ImGui::BulletText("File 1");
		ImGui::BulletText("File 2");
		ImGui::End();
	}
}