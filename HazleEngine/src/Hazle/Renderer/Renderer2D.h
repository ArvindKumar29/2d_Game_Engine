#pragma once

#include "Hazle/Core/Hazle.h"
#include "Texture.h"
#include "SubTexture2d.h"
#include "glm/glm.hpp"
#include "EditorCamera.h"
#include "Hazle/Scene/Component.h"
#include "Hazle/Core/Timestep.h"


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

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D> texture, const float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subTexture, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, const float tilingFactor = 1.0f, int entityID = -1);
		
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.001f, int entityID = -1);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);

		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::vec3& position, glm::vec2 size, const glm::vec4& color, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float thickness);

		static void DrawSprite(const glm::mat4& transform, CSpriteRenderer& sprite, int entityID);

		//static void CalculateStats(Timestep ts);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			//float lastFrameTime = 0;
			//float currentFrameTime = 0;
			//float FrameCalculateTime = 0;

			//uint32_t FPS = 0;
		};
		//static Timestep ts;

		static void ResetStats();
		static Statistics GetStats(); 
	private:
		static void FlushAndReset();
	};
}