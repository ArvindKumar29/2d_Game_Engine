#pragma once
#include "Renderer/RendererAPI.h"


namespace Hazle
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Init() override;
		virtual void DrawIndexed(const Hazle::Ref<Hazle::VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Hazle::Ref<Hazle::VertexArray>& vertexArray, uint32_t vertexCount = 0) override;
		virtual void SetLineWidth(float thickness) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;


	};
}