#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Hazle
{	
	class VertexArray;
	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Hazle::Ref<Hazle::Shader>& shader, const Hazle::Ref<Hazle::VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 VPMatrix;
		};

		static SceneData* m_SceneData;
	};
}