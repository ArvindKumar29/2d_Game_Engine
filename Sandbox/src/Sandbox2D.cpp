#include "Sandbox2D.h"


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),
	m_CameraController(1280.0f, 720.0f, true)
{}


void Sandbox2D::OnAttach()
{
	m_SquareVA = Hazle::VertexArray::Create();
	float vertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};

	uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };

	Hazle::Ref<Hazle::VertexBuffer> squareVB;
	squareVB = Hazle::VertexBuffer::Create(vertices, sizeof(vertices));
	//m_SquareVA->AddVertexBuffer(squareVB);
	Hazle::Ref<Hazle::IndexBuffer> squareIB;
	squareIB = Hazle::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);
	
	Hazle::BufferLayout layout2 = {
		{Hazle::ShaderDataType::Float3, "a_Position"},
	};
	squareVB->SetLayout(layout2);
	m_SquareVA->AddVertexBuffer(squareVB);
	
	m_Shader = Hazle::Shader::Create("Assets/Shaders/FlatColorShader.glsl");
	//m_TextureShader = Hazle::Shader::Create("Texture Shader", vertexSrc2, fragmentSrc2);

}

void Sandbox2D::OnDetach()
{

}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazle::Event & e)
{
	m_CameraController.OnEvent(e);
}

void Sandbox2D::OnUpdate(Hazle::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazle::RenderCommand::Clear();
	Hazle::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Hazle::Renderer::BeginScene(m_CameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::mat4 squareTransform;

	std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Hazle::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);
	
	Hazle::Renderer::Submit(m_Shader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hazle::Renderer::EndScene();
}