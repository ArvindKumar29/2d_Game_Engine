#include <hzpch.h>
#include "Hazle/ImGui/ImGuiLayer.h"
#include "Hazle/Core/Application.h"

#include <imgui.h>
#include "imguizmo.h"
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

		ImGui::StyleColorsDark();			// Set Dark Theme as default
		ImGui::StyleColorsClassic();		// Set Classic Theme as default

		// Setup Platform/Renderer bindings
		// Setting window rounding to 0.0f to avoid black corners when using multiple viewports.
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

		ImGui_ImplGlfw_InitForOpenGL(window, true);		// Initialize ImGui for GLFW
		ImGui_ImplOpenGL3_Init("#version 410");			// Initialize ImGui for OpenGL 3.0
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();						// Destroy ImGui context
	}

	void ImGuiLayer::begin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()					// End the Dear ImGui frame and render the frame at the end after updating the application
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());			// Render the ImGui draw data using OpenGL 3.0
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
			ImGuiIO& io = ImGui::GetIO();		// Get the ImGui IO object
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;			// If the event is in the mouse category and ImGui wants to capture the mouse, mark the event as handled.
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;	// If the event is in the keyboard category and ImGui wants to capture the keyboard, mark the event as handled.
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		ImGui::StyleColorsDark();
		auto& colors = ImGui::GetStyle().Colors;

		// BG - Deep cool obsidian slate (Icons ekdum pop karenge)
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.08f, 0.09f, 0.11f, 1.0f };

		// Headers - Cool dark slate with subtle purple-violet hover
		colors[ImGuiCol_Header] = ImVec4{ 0.14f, 0.15f, 0.19f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.24f, 0.20f, 0.35f, 1.0f }; // Matches folder pink/purple
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.20f, 0.18f, 0.30f, 1.0f };

		// Buttons - Sleek slate with violet synthwave hover tint
		colors[ImGuiCol_Button] = ImVec4{ 0.15f, 0.16f, 0.20f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.28f, 0.22f, 0.40f, 1.0f }; // Vibrant purple glow
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.20f, 0.18f, 0.30f, 1.0f };

		// FrameBg - Recessed deep dark box for inputs/checkboxes
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.06f, 0.06f, 0.08f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.14f, 0.15f, 0.19f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.18f, 0.19f, 0.25f, 1.0f };

		// Tabs - Active tab blends smoothly, hovered gets violet hint
		colors[ImGuiCol_Tab] = ImVec4{ 0.11f, 0.12f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.30f, 0.22f, 0.45f, 1.0f }; // Neon purple accent
		colors[ImGuiCol_TabActive] = ImVec4{ 0.18f, 0.19f, 0.24f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.09f, 0.10f, 0.12f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.14f, 0.15f, 0.19f, 1.0f };

		// Titles - Sleek dark top bars
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.08f, 0.09f, 0.11f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.12f, 0.13f, 0.17f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.08f, 0.09f, 0.11f, 1.0f };
	}

	void ImGuiLayer::SetLightThemeColors()
	{
		ImGui::StyleColorsLight();
		auto& colors = ImGui::GetStyle().Colors;

		// BG - Crisp modern ice-slate gray
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.94f, 0.95f, 0.97f, 1.0f };

		// Headers - Clean slate with soft pastel violet hover
		colors[ImGuiCol_Header] = ImVec4{ 0.86f, 0.88f, 0.92f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.78f, 0.82f, 0.95f, 1.0f }; // Soft violet-blue
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.72f, 0.76f, 0.90f, 1.0f };

		// Buttons - Ice gray with soft cyan/purple hover feedback
		colors[ImGuiCol_Button] = ImVec4{ 0.88f, 0.90f, 0.94f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.80f, 0.84f, 0.96f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.74f, 0.78f, 0.92f, 1.0f };

		// FrameBg - Pure white input boxes make text look super sharp!
		colors[ImGuiCol_FrameBg] = ImVec4{ 1.00f, 1.00f, 1.00f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.92f, 0.94f, 0.98f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.86f, 0.90f, 0.96f, 1.0f };

		// Tabs - Clean hierarchy
		colors[ImGuiCol_Tab] = ImVec4{ 0.88f, 0.90f, 0.94f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.76f, 0.80f, 0.95f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.94f, 0.95f, 0.97f, 1.0f }; // Blends into WindowBg
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.88f, 0.90f, 0.94f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.92f, 0.93f, 0.95f, 1.0f };

		// Titles
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.88f, 0.90f, 0.94f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.82f, 0.85f, 0.91f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.88f, 0.90f, 0.94f, 1.0f };
	}

}