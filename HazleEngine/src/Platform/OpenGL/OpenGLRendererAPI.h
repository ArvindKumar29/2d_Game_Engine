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
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;


	};
}