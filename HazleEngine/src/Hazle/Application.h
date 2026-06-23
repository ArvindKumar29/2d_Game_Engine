#pragma once
#include "core.h"
#include "Window.h"
#include "LayerStack.h" 
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Timestep.h"

namespace Hazle
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);


		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		void static Close();


	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	// To be defined in client
	Application* createApplication();
}

