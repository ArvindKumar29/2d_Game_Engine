#pragma once
#include <glm/glm.hpp>
#include "RendererAPI.h"

namespace Hazle
{
	class VertexArray;
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); };
		inline static void Clear(){ s_RendererAPI->Clear(); };
		inline static void Init()
		{
			s_RendererAPI->Init(); 
		};
		inline static void DrawIndexed(const Hazle::Ref<Hazle::VertexArray>& vertexArray, uint32_t indexCount = 0) 
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		};
		
		inline static void DrawLines(const Hazle::Ref<Hazle::VertexArray>& vertexArray, uint32_t vertexCount = 0) 
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		};

		inline static void SetLineWidth(float thickness)
		{
			s_RendererAPI->SetLineWidth(thickness);
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}