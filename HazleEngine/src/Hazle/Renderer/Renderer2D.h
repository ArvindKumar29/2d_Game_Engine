#pragma once

#include "Hazle/Renderer/OrthographicCamera.h"
#include "Hazle/Renderer/Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"


namespace Hazle
{
	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Shutdown();
		static void Flush();
		
		// Primitives

		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);
	};
}