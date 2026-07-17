#pragma once

#include "Hazle/Core/Hazle.h"
#include "Texture.h"
#include "SubTexture2d.h"
#include "glm/glm.hpp"
#include "EditorCamera.h"


namespace Hazle
{
	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera, glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Shutdown();
		static void Flush();
		static void StartBatch();

		// Primitives

		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<SubTexture2D> subTexture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<SubTexture2D> subTexture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D> texture, const float tilingFactor = 1.0f);
		
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
		};
		
		static void ResetStats();
		static Statistics GetStats(); 
	private:
		static void FlushAndReset();
	};
}