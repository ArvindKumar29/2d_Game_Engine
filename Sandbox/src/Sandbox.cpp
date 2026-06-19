#include <Hazle/Hazle.h>
//#include "imgui.h"

class ExampleLayer : public Hazle::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f), m_SquarePosition(0.0f), m_TrianglePosition(0.0f)
	{
		m_VertexArray.reset(Hazle::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.2f, 0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.5f, 0.7f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		Hazle::Ref<Hazle::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazle::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazle::BufferLayout layout = {
			{Hazle::ShaderDataType::Float3, "a_Position"},
			{Hazle::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazle::Ref<Hazle::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazle::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////// SQUARE PART JUST TO TEST INDEX BUFFER BINDING TO DIFFERENT VERTEX ARRAY ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_SquareVA.reset(Hazle::VertexArray::Create());
		float vertices2[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		};
		uint32_t indices2[6] = { 0, 1, 2, 1, 3, 2 };
		Hazle::Ref<Hazle::VertexBuffer> squareVB;
		squareVB.reset(Hazle::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		//m_SquareVA->AddVertexBuffer(squareVB);
		Hazle::Ref<Hazle::IndexBuffer> squareIB;
		squareIB.reset(Hazle::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		///////////////////////////////////////////////////
		Hazle::BufferLayout layout2 = {
			{Hazle::ShaderDataType::Float3, "a_Position"},
			{Hazle::ShaderDataType::Float2, "a_TexCoord"}
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
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
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
		m_Shader.reset(Hazle::Shader::Create(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_VP;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
			} 
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 f_Color;
			
			in vec3 v_Position;
			uniform vec3 u_Color;

			void main()
			{
				f_Color = vec4(u_Color, 1.0);					
			}
		)";

		m_Shader2.reset(Hazle::Shader::Create(vertexSrc2, fragmentSrc2));
		//m_TextureShader.reset(Hazle::Shader::Create(vertexSrc2, fragmentSrc2));

		m_TextureShader.reset(Hazle::Shader::Create("Assets/Shaders/Texture.glsl"));
		
		m_Texture = Hazle::Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_ArvindSignTexture = Hazle::Texture2D::Create("Assets/Textures/sign2.png");

		std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

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
		if (Hazle::Input::IsKeyPressed(Hazle::Key::A))
		{
			m_SquarePosition.x -= m_SquareSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::D))
		{
			m_SquarePosition.x += m_SquareSpeed * ts;
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::W))
		{
			m_SquarePosition.y += m_SquareSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::S))
		{
			m_SquarePosition.y -= m_SquareSpeed * ts;
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::J))
		{
			m_TrianglePosition.x -= m_TriangleSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::L))
		{
			m_TrianglePosition.x += m_TriangleSpeed * ts;
		}
		if (Hazle::Input::IsKeyPressed(Hazle::Key::I))
		{
			m_TrianglePosition.y += m_TriangleSpeed * ts;
		}
		else if (Hazle::Input::IsKeyPressed(Hazle::Key::K))
		{
			m_TrianglePosition.y -= m_TriangleSpeed * ts;
		}


		Hazle::RenderCommand::Clear();
		Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(glm::radians(m_CameraRotation));

		Hazle::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		//glm::mat4 triangleTransform;
		glm::mat4 squareTransform;

		std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader2)->UploadUniformFloat3("u_Color", m_SquareColor);


		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::vec3 pos(j * 0.11f, i * 0.11f, 0.0f);
				squareTransform = glm::translate(glm::mat4(1.0f), pos + m_SquarePosition) * scale;
				//triangleTransform = glm::translate(glm::mat4(1.0f), pos + m_TrianglePosition) * scale;
				Hazle::Renderer::Submit(m_Shader2, m_SquareVA, squareTransform);
				//Hazle::Renderer::Submit(m_Shader, m_VertexArray, triangleTransform);
				
			}
		}

		m_Texture->Bind(0); 
		Hazle::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ArvindSignTexture->Bind(0);
		Hazle::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//triangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition) * scale;
		//Hazle::Renderer::Submit(m_Shader, m_VertexArray);
		Hazle::Renderer::EndScene();
	}
	 
	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
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

	Hazle::Ref<Hazle::Shader> m_Shader;
	Hazle::Ref<Hazle::Shader> m_Shader2, m_TextureShader;
	Hazle::Ref<Hazle::VertexArray> m_VertexArray;
	Hazle::Ref<Hazle::VertexArray> m_SquareVA;
	Hazle::OrthographicCamera m_Camera;

	Hazle::Ref<Hazle::Texture2D> m_Texture, m_ArvindSignTexture;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 3.0f;
	
	float m_CameraRotation;
	float m_RotationSpeed = 10.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareSpeed = 1.0f;

	glm::vec3 m_TrianglePosition;
	float m_TriangleSpeed = 1.0f;

	glm::vec3 m_SquareColor;

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