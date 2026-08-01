#pragma once
#include "Hazle/Core/core.h"
#include "Hazle/Core/Window.h"
#include "Hazle/Core/LayerStack.h"
#include "Hazle/Event/ApplicationEvent.h"
#include "Hazle/ImGui/ImGuiLayer.h"
#include "Hazle/Core/Timestep.h"
#include "Hazle/Scene/Audio.h"

namespace Hazle
{
	class Application
	{
	public:
		Application(const std::string name = "Hazle Engine");
		virtual ~Application();
		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);


		inline Window& GetWindow() { return *m_Window; }
		void close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		void static Close();


	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
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

