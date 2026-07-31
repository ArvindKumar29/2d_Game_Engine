#include <hzpch.h>
#include "EditorLayer.h"
#include "Hazle/Core/Hazle.h"
#include "Hazle/Scene/SceneSerializer.h"
#include "Hazle/utils/PlatformUtils.h"
#include "imguizmo.h"
#include "Hazle/Math/Math.h"
#include "Hazle/ImGui/ImGuiLayer.h"

namespace Hazle
{
	extern const std::filesystem::path g_AssetPath;


	EditorLayer::EditorLayer()
		:Layer("EditorLayer"),
		m_CameraController(1280.0f, 720.0f, true)
	{}


	void EditorLayer::OnAttach()
	{
		m_CameraController.SetZoomLevel(10.0f);

		FrameBufferSpecifications fbSpec;
		fbSpec.Attachements = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1920;
		fbSpec.Height = 1080;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		// Loading HazleUI Icons
		m_IconPlay		= Texture2D::Create("assets/textures/play.png");
		m_IconStop		= Texture2D::Create("assets/textures/stop.png");
		m_IconSimulate	= Texture2D::Create("assets/textures/simulate.png");
		m_IconSun		= Texture2D::Create("assets/textures/sun.png");
		m_IconMoon		= Texture2D::Create("assets/textures/moon.png");
		m_IconSave		= Texture2D::Create("assets/textures/save.png");


		//SCENE
		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		//ENTITIES
		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddOrReplaceComponent<CTransform>();
		m_SquareEntity.AddOrReplaceComponent<CSpriteRenderer>();

		Entity redSquare = m_ActiveScene->CreateEntity("RedSquare");
		redSquare.AddOrReplaceComponent<CTransform>();
		redSquare.AddOrReplaceComponent<CSpriteRenderer>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddOrReplaceComponent<CCamera>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		m_CameraEntity.getComponent<CCamera>().Primary = true;
		m_CameraEntity.AddOrReplaceComponent<CTransform>();
		
		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		m_SecondCamera.AddOrReplaceComponent<CCamera>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		m_SecondCamera.AddOrReplaceComponent<CTransform>();

		
		
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& transform = getComponent<CTransform>().GetTransform();
			}

			void OnDestroy()
			{}

			void OnUpdate(Timestep ts)
			{
				auto& translation = getComponent<CTransform>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * ts;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(Key::E))
					translation.z -= speed * ts;
				if (Input::IsKeyPressed(Key::F))
					translation.z += speed * ts;
				
			}
		};
		m_CameraEntity.AddOrReplaceComponent<CNativeScript>().Bind<CameraController>();
		m_SecondCamera.AddOrReplaceComponent<CNativeScript>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		//SceneSerializer serializer(m_ActiveScene);
		//serializer.DeSerialize("Assets/Scenes/Example.hz");
	}

	void EditorLayer::OnDetach()
	{}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float size = ImGui::GetWindowHeight() - 4.0f;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// ADDING THEME CHANGE BUTTON...
		Ref<Texture2D> icon = m_EditorTheme == EditorTheme::Dark ? m_IconSun : m_IconMoon;
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - size - 5.0f);
		if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { size, size }, { 0, 1 }, { 1, 0 }))
		{
			
			if (m_EditorTheme == EditorTheme::Dark)
			{
				Hazle::ImGuiLayer::SetLightThemeColors();
				m_EditorTheme = EditorTheme::Light;
			}
			else if (m_EditorTheme == EditorTheme::Light)
			{
				Hazle::ImGuiLayer::SetDarkThemeColors();
				m_EditorTheme = EditorTheme::Dark;
			}
			
		}
		
		// ADDING PLAY AND PAUSE BUTTON...
		icon = m_SceneState != SceneState::Play ? m_IconPlay : m_IconStop;
		ImGui::SameLine((ImGui::GetContentRegionAvail().x - size) * 0.5f - size);
		if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { size, size }, { 0, 1 }, { 1, 0 }))
		{
			if (m_ActiveScene)
			{
				if (m_SceneState == SceneState::Edit)
					OnScenePlay();
				else if (m_SceneState == SceneState::Simulate)
				{
					OnSceneStop();
					OnScenePlay();
				}
				else
					OnSceneStop();

			}
		}

		//ADDING SIMULATE BUTTON
		icon = m_SceneState != SceneState::Simulate ? m_IconSimulate : m_IconStop;
		ImGui::SameLine((ImGui::GetContentRegionAvail().x - size) * 0.5f + size);
		if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { size, size }, { 0, 1 }, { 1, 0 }))
		{
			if(m_ActiveScene)
			{

				if (m_SceneState == SceneState::Edit)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Play)
				{
					OnSceneStop();
					OnSceneSimulate();
				}
				else
					OnSceneStop();
			}
		}

		ImGui::PopStyleColor();
		//ImGui::PopStyleVar(2);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_RuntimeScene = Scene::Copy(m_EditorScene);
		m_RuntimeScene->OnRuntimeStart();

		m_ActiveScene = m_RuntimeScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	
	void EditorLayer::OnSceneSimulate()
	{
		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		HZ_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);
		if (m_SceneState == SceneState::Play)
		{
			m_ActiveScene->OnRuntimeStop();
			m_RuntimeScene = nullptr;
		}
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();
		

		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	}


	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPresedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPresedEvent));
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}


	bool EditorLayer::OnKeyPresedEvent(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;
		bool controlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::S:
			{
				if (controlPressed && shiftPressed)
					SaveSceneAs();
				else if (controlPressed)
					SaveScene();
				break;
			}
			case Key::N:
			{
				if (controlPressed)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (controlPressed)
					OpenScene();
				break;
			}
			case Key::D:
			{
				if (controlPressed)
					OnDuplicateEntity();
				break;
			}

			case Key::Q:
			{
				m_GizmoType = -1;
				break;
			}
			
			case Key::W:
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			
			case Key::E:
			{
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			
			case Key::R:
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
		}
		return true;
	}

	bool EditorLayer::OnMouseButtonPresedEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if(m_ViewportHovered && !ImGuizmo::IsOver()  && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			Renderer2D::BeginScene(camera.getComponent<CCamera>().camera, camera.getComponent<CTransform>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			//CIRCLE COLLIDERS
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<CTransform, CCircleCollider2D>();
				for (auto entity : view)
				{
					auto transform = view.get<CTransform>(entity);
					auto cc2d = view.get<CCircleCollider2D>(entity);

					glm::vec3 translation = transform.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = transform.Scale * glm::vec3(cc2d.Radius * 2.0f);


					glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(trans, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}

			//BOX COLLIDERS
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<CTransform, CBoxCollider2D>();
				for (auto entity : view)
				{
					auto transform = view.get<CTransform>(entity);
					auto bc2d = view.get<CBoxCollider2D>(entity);

					glm::vec3 translation = transform.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = transform.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
					float rotation = transform.Rotation.z;

					glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(trans, glm::vec4(0, 1, 0, 1));
				}
			}
		}
		Renderer2D::EndScene();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{

		if (Input::IsKeyPressed(Key::Escape))
		{
			Application::Get().Close();
		}

		//Resize
		FrameBufferSpecifications spec = m_FrameBuffer->GetSpecifications();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		
		

		// Render
		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1 before updating scene
		m_FrameBuffer->ClearColorAttachments(1, -1);

		// Update
		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportFocused)
					m_CameraController.OnUpdate(ts);
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
			case SceneState::Simulate:
			{
				m_ActiveScene->OnUpdateSimulation(m_EditorCamera, ts);
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}
		
		OnOverlayRender();


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
		ImGuiStyle& style = ImGui::GetStyle();
		float minwinsizex = style.WindowMinSize.x;
		float minwinsizey = style.WindowMinSize.y;
		style.WindowMinSize.x = 200.0f;
		style.WindowMinSize.y = 25.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minwinsizex;
		style.WindowMinSize.y = minwinsizey;


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				
				if (ImGui::MenuItem("Save As..." , "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Save..." , "Ctrl+S"))
				{
					SaveScene();
				}
				
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Engine Statistics");

		std::string name = "None";
		if (m_HoveredEntity && m_HoveredEntity.hasComponent<CTag>())
			name = m_HoveredEntity.getComponent<CTag>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.QuadCount * 4);
		ImGui::Text("Indices: %d", stats.QuadCount * 6);
		
		ImGui::Separator();

		ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
		//ImGui::DragFloat3("Camera Trasnform: ",
		//	glm::value_ptr(m_PrimaryCameraptr.getComponent<CTransform>().Translation));
		ImGui::Separator();

		m_ProfileResults.clear();
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Viewport");

		auto viewportOffset = ImGui::GetCursorPos(); // Include tab bar


		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0.0f)
		{
			m_FrameBuffer->Resize(uint32_t(viewportPanelSize.x), uint32_t(viewportPanelSize.y));
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			m_EditorCamera.SetViewportSize(viewportPanelSize.x, viewportPanelSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		
		if (ImGui::BeginDragDropTarget())
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;

				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}

			ImGui::EndDragDropTarget();
		}

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		
		ImGuizmo::BeginFrame();
		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetDrawlist();

			ImVec2 windowPos	 = ImGui::GetWindowPos();
			ImVec2 minRegion	 = ImGui::GetWindowContentRegionMin();
			ImVec2 maxRegion	 = ImGui::GetWindowContentRegionMax();

			float viewportX		 = windowPos.x + minRegion.x;
			float viewportY		 = windowPos.y + minRegion.y;
			float viewportWidth  = maxRegion.x - minRegion.x;
			float viewportHeight = maxRegion.y - minRegion.y;
			ImGuizmo::SetRect(viewportX, viewportY, viewportWidth, viewportHeight);

			// CAMERA
			glm::mat4 cameraProjection;
			glm::mat4 cameraView;
			 
			// Runtime Camera from entity
			Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			if(cameraEntity && cameraEntity.hasComponent<CCamera>())
			{
				// Entity Camera
				const auto& camera = cameraEntity.getComponent<CCamera>();
				
				bool isOrthographic = camera.camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic;
				ImGuizmo::SetOrthographic(isOrthographic);

				cameraProjection = camera.camera.GetProjection();
				cameraView = glm::inverse(cameraEntity.getComponent<CTransform>().GetTransform());
			}
			else
			{
				//bool isOrthographic = m_EditorCamera.GetProjectionType();
				ImGuizmo::SetOrthographic(false);				
				cameraProjection = m_EditorCamera.GetProjection();
				cameraView = m_EditorCamera.GetViewMatrix();
			}

			if (selectedEntity.hasComponent<CTransform>())
			{
				// Entity TransfoSrm
				auto& tc = selectedEntity.getComponent<CTransform>();
				glm::mat4 transform = tc.GetTransform();

				// snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f;
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					bool decomposedTransform = Math::DecomposeTransform(transform, translation, rotation, scale);

					if(decomposedTransform)
					{
						//glm::vec3 deltaRotation = rotation - tc.Rotation;

						tc.Translation = translation;
						tc.Rotation = rotation;
						tc.Scale = scale;
					}
				}
			}
		}

		UI_Toolbar();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".hz")
		{
			HZ_CORE_WARN("Could not load {0} - not a hazle scene file", path.filename().string());
			return;
		}

		m_EditorScene = CreateRef<Scene>();
		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_EditorScene);
		SceneSerializer serializer(m_EditorScene);
		if (serializer.DeSerialize(path.string()))
			m_ActiveScene = m_EditorScene;

		currentPath = path.string();
	}

	void EditorLayer::OpenScene()
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		std::string filepath = FileDialogs::OpenFile("Hazle Scene (*.hz)\0*.hz\0"); // Hazle Scene (.hz) this will show up upto first \0 and .hz is the actual file type from first \0 to second \0
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazle Scene (*.hz)\0*.hz\0"); // Hazle Scene (.hz) this will show up upto first \0 and .hz is the actual file type from first \0 to second \0
		if (!filepath.empty())
		{
			SceneSerializer seriazlizer(m_ActiveScene);
			seriazlizer.Serialize(filepath);
		}
		//ImGui::Image((ImTextureID)(uint64_t)m_IconSave->GetRendererID(), { 20.0f, 20.0f }, { 1, 0 }, { 0, 1 });
	}
	
	void EditorLayer::SaveScene()
	{
		std::string filepath = currentPath; // Hazle Scene (.hz) this will show up upto first \0 and .hz is the actual file type from first \0 to second \0
		if (!filepath.empty())
		{
			SceneSerializer seriazlizer(m_ActiveScene);
			seriazlizer.Serialize(filepath);

			//ImGui::BeginPopup("Scene Saved!!!");
			//ImGui::EndPopup();
		}
		//ImGui::Image((ImTextureID)(uint64_t)m_IconSave->GetRendererID(), { 20.0f, 20.0f }, { 1, 0 }, { 0, 1 });

	}
}