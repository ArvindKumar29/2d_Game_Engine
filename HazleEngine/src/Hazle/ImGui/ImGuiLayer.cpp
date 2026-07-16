#include <hzpch.h>
#include "Hazle/ImGui/ImGuiLayer.h"
#include "Hazle/Core/Application.h"

#include <imgui.h>
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_API

namespace Hazle {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer()
	{}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;        // Disable merging of viewports
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;      // Disable adding the ImGui viewports to the taskbar
	
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Kameron-Bold.ttf", 18.5f);

		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Kameron-Regular.ttf", 18.5f);

		ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();
		//SetLightThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		//BG
		colors[ ImGuiCol_WindowBg ]			= ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		//Headers
		colors[ ImGuiCol_Header ]				= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ ImGuiCol_HeaderHovered ]		= ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ ImGuiCol_HeaderActive ]			= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		
		//Buttons
		colors[ ImGuiCol_Button ]				= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ ImGuiCol_ButtonHovered ]		= ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ ImGuiCol_ButtonActive ]			= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	
		//FrameBG
		colors[ ImGuiCol_FrameBg ]				= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ ImGuiCol_FrameBgHovered ]		= ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ ImGuiCol_FrameBgActive ]		= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		//Tabs
		colors[ ImGuiCol_Tab ]					= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ ImGuiCol_TabHovered ]			= ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ ImGuiCol_TabActive ]			= ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ ImGuiCol_TabUnfocused ]			= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ ImGuiCol_TabUnfocusedActive ]	= ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		//Titles
		colors[ ImGuiCol_TitleBg ]				= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ ImGuiCol_TitleBgActive ]		= ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ ImGuiCol_TitleBgCollapsed ]		= ImVec4{ 0.95f, 0.1505f, 0.951f, 1.0f };
	}

	void ImGuiLayer::SetLightThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		// BG
		colors[ImGuiCol_WindowBg]			= ImVec4{ 0.5f, 0.605f, 0.7f, 1.0f };

		//Headers
		colors[ImGuiCol_Header]				= ImVec4{ 0.2f, 0.7f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered]		= ImVec4{ 0.3f, 0.75f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive]		= ImVec4{ 0.15f, 0.7f, 0.7f, 1.0f };

		//Buttons
		colors[ImGuiCol_Button]				= ImVec4{ 0.7f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered]		= ImVec4{ 0.9f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive]		= ImVec4{ 0.7f, 0.1505f, 0.151f, 0.7f };

		//FrameBG
		colors[ImGuiCol_FrameBg]			= ImVec4{ 0.7f -0.2f, 0.7f - 0.205f, 0.7f - 0.21f, 0.7f };
		colors[ImGuiCol_FrameBgHovered]		= ImVec4{ 0.7f -0.3f, 0.7f - 0.305f, 0.7f - 0.31f, 0.7f };
		colors[ImGuiCol_FrameBgActive]		= ImVec4{ 0.7f -0.15f,0.7f -  0.1505f, 0.7f - 0.151f, 0.7f };

		//Tabs
		colors[ImGuiCol_Tab]				= ImVec4{ 0.7f -0.15f,0.7f -  0.1505f, 0.7f - 0.151f, 0.7f };
		colors[ImGuiCol_TabHovered]			= ImVec4{ 0.7f -0.38f,0.7f -  0.3805f, 0.7f - 0.381f, 0.7f };
		colors[ImGuiCol_TabActive]			= ImVec4{ 0.7f -0.28f,0.7f -  0.2805f, 0.7f - 0.281f, 0.7f };
		colors[ImGuiCol_TabUnfocused]		= ImVec4{ 0.7f -0.15f,0.7f -  0.1505f, 0.7f - 0.151f, 0.7f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.7f -0.2f, 0.7f - 0.205f, 0.7f - 0.21f, 0.7f };

		//Titles
		colors[ImGuiCol_TitleBg]			= ImVec4{ 0.7f -0.15f,0.7f -  0.1505f, 0.7f - 0.151f, 0.7f };
		colors[ImGuiCol_TitleBgActive]		= ImVec4{ 0.7f -0.15f,0.7f -  0.1505f, 0.7f - 0.151f, 0.7f };
		colors[ImGuiCol_TitleBgCollapsed]	= ImVec4{ 0.7f -0.95f,0.7f -  0.1505f, 0.7f - 0.951f, 0.7f };
	}

}