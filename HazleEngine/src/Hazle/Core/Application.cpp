#include <hzpch.h>

#include "Application.h"
#include "Hazle/Core/Log.h"
#include "Hazle/Core/Input.h"
#include "Hazle/Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "Hazle/Scene/Audio.h"


namespace Hazle
{
	// defining a binding fuction for the application class to use in the event callback and event dispatcher
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr; // initializing the static instance pointer to nullptr

	Application::Application(const std::string name)
	{
		HZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this; // setting the static instance pointer to this instance of the application
		m_Window = Window::Create(WindowProps(name)); // creating a window with the given name
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); // setting the event callback for the window to the OnEvent function of this application instance
		m_Window->SetVSync(false); // disabling vertical synchronization for the window

		Renderer::Init(); // initializing the renderer
		Audio::Init(); // initializing the audio system

		m_ImGuiLayer = new ImGuiLayer(); // creating a new ImGuiLayer instance
		PushOverlay(m_ImGuiLayer); // pushing the ImGuiLayer onto the layer stack as an overlay
	}

	Application::~Application()
	{
		s_Instance = nullptr; // resetting the static instance pointer to nullptr
		Audio::Shutdown(); // shutting down the audio system
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer); // pushing the given layer onto the layer stack
		layer->OnAttach(); // calling the OnAttach function of the layer to perform any necessary initialization
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer); // pushing the given layer onto the layer stack as an overlay
		layer->OnAttach(); // calling the OnAttach function of the layer to perform any necessary initialization
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e); // creating an EventDispatcher instance to dispatch the event to the appropriate handler function
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); // dispatch WindowCloseEvent to the OnWindowClose function of this application instance
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize)); // dispatch WindowResizeEvent to the OnWindowResize function of this application instance
		
		//HZ_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) // iterating through the layer stack in reverse order to propagate the event to each layer
		{
			(*--it)->OnEvent(e); // calling the OnEvent function of the layer to handle the event
			if (e.Handled) // if the event has been handled by a layer, stop propagating the event to other layers it prevents the event from being handled by multiple layers
				break;
		}
	}

	// the main loop of the application, which runs until the application is closed
	void Application::run() 
	{
		while(m_Running) 
		{
			float time = (float)glfwGetTime(); // getting the current time in seconds since the application started for delta time calculation and timesteps
			Timestep deltatime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) // if the application window is not minimized, update each layer in the layer stack and if it is minimized, skip the update to save resources
			{
				for (Layer* layer : m_LayerStack) 
					layer->OnUpdate(deltatime);
			}

			m_ImGuiLayer->begin(); // begin the ImGui frame for rendering the GUI
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(); // calling the OnImGuiRender function of each layer to render the GUI elements for that layer
			m_ImGuiLayer->end();

			auto [x, y] = Input::GetMousePosition(); // getting the current mouse position for debugging purposes
			//HZ_TRACE("{0}, {1}", x, y);
			
			m_Window->OnUpdate(); // updating the window to process any pending events and swap the front and back buffers
			Audio::Update(); //	updating the audio system to process any pending audio events and update the audio state
		}
	}

	void Application::Close()
	{
		s_Instance->m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}