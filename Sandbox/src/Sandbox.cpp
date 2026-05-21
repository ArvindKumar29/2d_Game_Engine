#include <Hazle/Hazle.h>

class ExampleLayer : public Hazle::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition({ 0.0f, 0.0f, 0.0f }), m_CameraRotation(0.0f)
	{
		m_VertexArray.reset(Hazle::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.2f, 0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.5f, 0.7f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Hazle::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazle::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazle::BufferLayout layout = {
			{Hazle::ShaderDataType::Float3, "a_Position"},
			{Hazle::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazle::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazle::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////// SQUARE PART JUST TO TEST INDEX BUFFER BINDING TO DIFFERENT VERTEX ARRAY ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_SquareVA.reset(Hazle::VertexArray::Create());
		float vertices2[3 * 4] = {
			-0.7f, -0.7f, 0.0f,
			 0.7f, -0.7f, 0.0f,
			-0.7f,  0.7f, 0.0f,
			 0.7f,  0.7f, 0.0f,
		};
		uint32_t indices2[6] = { 0, 1, 2, 1, 3, 2 };
		std::shared_ptr<Hazle::VertexBuffer> squareVB;
		squareVB.reset(Hazle::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		m_SquareVA->AddVertexBuffer(squareVB);
		std::shared_ptr<Hazle::IndexBuffer> squareIB;
		squareIB.reset(Hazle::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		///////////////////////////////////////////////////
		Hazle::BufferLayout layout2 = {
			{Hazle::ShaderDataType::Float3, "a_Position"},
		};
		squareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(squareVB);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_VP;
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			} 
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 f_Color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				f_Color = vec4(v_Color.rgb, 1.0);					
			}
		)";
		m_Shader.reset(new Hazle::Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_VP;
			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			} 
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 f_Color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				f_Color = vec4(1.0, 0.0, 0.0, 0.0);					
			}
		)";

		m_Shader2.reset(new Hazle::Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Hazle::Timestep ts) override
	{
		HZ_CORE_TRACE("Frame time: {0}s and {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		if (Hazle::Input::IsKeyPressed(Hazle::Key::Escape))
		{
			Hazle::Application::Get().GetWindow().Close();
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::Left))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::Right))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::Up))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::Down))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::Q))
		{
			m_CameraRotation += m_RotationSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::E))
		{
			m_CameraRotation -= m_RotationSpeed * ts;
		}


		Hazle::RenderCommand::Clear();
		Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(glm::radians(m_CameraRotation));

		Hazle::Renderer::BeginScene(m_Camera);
		Hazle::Renderer::Submit(m_Shader2, m_SquareVA);
		Hazle::Renderer::Submit(m_Shader, m_VertexArray);
		Hazle::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
	
	}

	void OnEvent(Hazle::Event& event) override
	{
		Hazle::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazle::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Hazle::KeyPressedEvent& event)
	{
		
		return false;
	}

private:
	std::shared_ptr<Hazle::Shader> m_Shader;
	std::shared_ptr<Hazle::Shader> m_Shader2;
	std::shared_ptr<Hazle::VertexArray> m_VertexArray;
	std::shared_ptr<Hazle::VertexArray> m_SquareVA;
	Hazle::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.5f;
	
	float m_CameraRotation;
	float m_RotationSpeed = 0.5f;
};

class sandbox : public Hazle::Application 
{

public:
	sandbox() 
	{
		PushLayer(new ExampleLayer());
	};
	~sandbox() 
	{
	};

};

Hazle::Application* Hazle::createApplication() 
{
	return new sandbox();
}