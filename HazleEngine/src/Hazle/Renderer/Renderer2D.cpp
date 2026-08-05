#include <hzpch.h>
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Hazle/Core/Log.h"

namespace Hazle
{
	//Timestep Renderer2D::ts;

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;

		//Edirtor Only
		int EntityID;
	};
	
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		//Edirtor Only
		int EntityID;
	};
	
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		
		//Edirtor Only
		int EntityID;
	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 1000000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		//Quads
		Ref<VertexArray> m_QuadVA;
		Ref<VertexBuffer> m_QuadBuffer;
		Ref<Shader> m_TextureShader;
		Ref<Texture2D> m_WhiteTexture;
		uint32_t m_QuadIndexCount = 0;
		QuadVertex* m_QuadVertexBufferBase = nullptr;
		QuadVertex* m_QuadVertexBufferPtr = nullptr;
		
		//Circles
		Ref<VertexArray> m_CircleVA;
		Ref<VertexBuffer> m_CircleBuffer;
		Ref<Shader> m_CircleShader;
		uint32_t m_CircleIndexCount = 0;
		CircleVertex* m_CircleVertexBufferBase = nullptr;
		CircleVertex* m_CircleVertexBufferPtr = nullptr;
		
		//Lines
		Ref<VertexArray> m_LineVA;
		Ref<VertexBuffer> m_LineBuffer;
		Ref<Shader> m_LineShader;
		uint32_t m_LineVertexCount = 0;
		LineVertex* m_LineVertexBufferBase = nullptr;
		LineVertex* m_LineVertexBufferPtr = nullptr;
		float thickness = 2.0f;


		std::array<Ref<Texture2D>, MaxTextureSlots> m_TextureSlots;
		uint32_t m_TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 m_QuadVertexPositions[4];
		
		Renderer2D::Statistics m_Stats;
	};


	Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// Quads
		s_Data.m_QuadVA = VertexArray::Create();
		s_Data.m_QuadBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.m_QuadBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"    },
			{ShaderDataType::Float2, "a_TexCoord"    },
			{ShaderDataType::Float4, "a_Color"       },
			{ShaderDataType::Float,  "a_TexIndex"    },
			{ShaderDataType::Float,  "a_TilingFactor"},
			{ShaderDataType::Int,    "a_EntityID"    }
			});
		s_Data.m_QuadVA->AddVertexBuffer(s_Data.m_QuadBuffer);
		s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = offset + 0;
			QuadIndices[i + 1] = offset + 1;
			QuadIndices[i + 2] = offset + 2;

			QuadIndices[i + 3] = offset + 2;
			QuadIndices[i + 4] = offset + 3;
			QuadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> QuadIB;
		QuadIB = IndexBuffer::Create(QuadIndices, s_Data.MaxIndices);
		s_Data.m_QuadVA->SetIndexBuffer(QuadIB);
		delete[] QuadIndices;

		// Circles
		s_Data.m_CircleVA = VertexArray::Create();
		s_Data.m_CircleBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.m_CircleBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition" },
			{ShaderDataType::Float3, "a_LocalPosition" },
			{ShaderDataType::Float4, "a_Color"		   },
			{ShaderDataType::Float,  "a_Thickness"	   },
			{ShaderDataType::Float,  "a_Fade"		   },
			{ShaderDataType::Int,    "a_EntityID"	   }
			});
		s_Data.m_CircleVA->AddVertexBuffer(s_Data.m_CircleBuffer);
		s_Data.m_CircleVA->SetIndexBuffer(QuadIB); // Use quadIB as it has the same parameters
		s_Data.m_CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
		
		// Lines
		s_Data.m_LineVA = VertexArray::Create();
		s_Data.m_LineBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.m_LineBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float4, "a_Color"	  },
			{ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.m_LineVA->AddVertexBuffer(s_Data.m_LineBuffer);
		//s_Data.m_LineVA->SetIndexBuffer(QuadIB); // Use quadIB as it has the same parameters
		s_Data.m_LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xffffffff;
		s_Data.m_WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));


		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.m_TextureShader = Shader::Create("Assets/Shaders/Quad.glsl");
		s_Data.m_CircleShader = Shader::Create("Assets/Shaders/Circle.glsl");
		s_Data.m_LineShader = Shader::Create("Assets/Shaders/Line.glsl");
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			s_Data.m_TextureSlots[i] = 0;
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;

		s_Data.m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::BeginScene(const Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		//Quads
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_VP", viewProj);
		
		//Circles
		s_Data.m_CircleShader->Bind();
		s_Data.m_CircleShader->SetMat4("u_VP", viewProj);
		
		//Lines
		s_Data.m_LineShader->Bind();
		s_Data.m_LineShader->SetMat4("u_VP", viewProj);
		
		StartBatch();

		//HZ_CORE_WARN("SCENE STARTED WITH CAMERA!!!");
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		//Quads
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_VP", camera.GetVPMatrix());
		
		//Circles
		s_Data.m_CircleShader->Bind();
		s_Data.m_CircleShader->SetMat4("u_VP", camera.GetVPMatrix());
		
		//Lines
		s_Data.m_LineShader->Bind();
		s_Data.m_LineShader->SetMat4("u_VP", camera.GetVPMatrix());
		
		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		glm::mat4 viewProj = camera.GetViewProjection();

		//Quads
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_VP", viewProj);

		//Circles
		s_Data.m_CircleShader->Bind();
		s_Data.m_CircleShader->SetMat4("u_VP", viewProj);
		
		//Lines
		s_Data.m_LineShader->Bind();
		s_Data.m_LineShader->SetMat4("u_VP", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.m_QuadVertexBufferPtr - (uint8_t*)s_Data.m_QuadVertexBufferBase);
		s_Data.m_QuadBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.m_QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.m_QuadVertexBufferPtr - (uint8_t*)s_Data.m_QuadVertexBufferBase);
			s_Data.m_QuadBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);
			
			for (uint32_t i = 0; i < s_Data.m_TextureSlotIndex; i++)
				s_Data.m_TextureSlots[i]->Bind(i);

			s_Data.m_TextureShader->Bind();
			RenderCommand::DrawIndexed(s_Data.m_QuadVA, s_Data.m_QuadIndexCount);
			s_Data.m_Stats.DrawCalls++;
		}

		if (s_Data.m_CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.m_CircleVertexBufferPtr - (uint8_t*)s_Data.m_CircleVertexBufferBase);
			s_Data.m_CircleBuffer->SetData(s_Data.m_CircleVertexBufferBase, dataSize);
			
			s_Data.m_CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.m_CircleVA, s_Data.m_CircleIndexCount);
			s_Data.m_Stats.DrawCalls++;
		}
		
		if (s_Data.m_LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.m_LineVertexBufferPtr - (uint8_t*)s_Data.m_LineVertexBufferBase);
			s_Data.m_LineBuffer->SetData(s_Data.m_LineVertexBufferBase, dataSize);
			
			s_Data.m_LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.thickness);
			RenderCommand::DrawLines(s_Data.m_LineVA, s_Data.m_LineVertexCount);
			s_Data.m_Stats.DrawCalls++;
		}
	}

	void Renderer2D::StartBatch()
	{
		s_Data.m_WhiteTexture->Bind(0);
		
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;
		
		s_Data.m_CircleIndexCount = 0;
		s_Data.m_CircleVertexBufferPtr = s_Data.m_CircleVertexBufferBase;
		
		s_Data.m_LineVertexCount = 0;
		s_Data.m_LineVertexBufferPtr = s_Data.m_LineVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.m_QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const glm::vec4& color)
	{

		glm::mat4& transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const float rotation, const glm::vec2 & size, const Ref<Texture2D> texture, const glm::vec4 & color, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, texture, color, tilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color, const float tilingFactor)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, texture, tilingFactor);

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2& size, const Ref<SubTexture2D> subTexture, const glm::vec4& color, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, size, subTexture, color, tilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3 & position, const float rotation, const glm::vec2 & size, const Ref<SubTexture2D> subTexture, const glm::vec4 & color, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		constexpr glm::vec2 textureCoord[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		if (s_Data.m_QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
			s_Data.m_QuadVertexBufferPtr->Color = color;
			s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[i];
			s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.m_QuadVertexBufferPtr->EntityID = entityID;
			s_Data.m_QuadVertexBufferPtr++;
			//HZ_CORE_WARN("dRAWING QUAD!!!");
		}
		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D> texture, const float tilingFactor, int entityID)
	{
		constexpr glm::vec2 textureCoord[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		if (s_Data.m_QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		for (uint32_t i = 1; i < s_Data.m_TextureSlotIndex; i++)
		{
			if (s_Data.m_TextureSlots[i].get() == texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
			s_Data.m_QuadVertexBufferPtr->Color = color;
			s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[i];
			s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.m_QuadVertexBufferPtr->EntityID = entityID;
			s_Data.m_QuadVertexBufferPtr++;
		}
		
		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subTexture, const glm::vec4& color, const float tilingFactor, int entityID)
	{
		const glm::vec2* textureCoord = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_Data.m_QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.m_TextureSlotIndex; i++)
		{
			if (s_Data.m_TextureSlots[i].get() == texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[i];
			s_Data.m_QuadVertexBufferPtr->Color = color;
			s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[i];
			s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.m_QuadVertexBufferPtr->EntityID = entityID;
			s_Data.m_QuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		// TODO: TO BE IMPLEMENTED FOR CIRCLES SEPERATELY
		if (s_Data.m_QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();


		for (size_t i = 0; i < 4; i++)
		{
			s_Data.m_CircleVertexBufferPtr->WorldPosition = transform * s_Data.m_QuadVertexPositions[i];
			s_Data.m_CircleVertexBufferPtr->LocalPosition = s_Data.m_QuadVertexPositions[i] * 2.0f;
			s_Data.m_CircleVertexBufferPtr->Color = color;
			s_Data.m_CircleVertexBufferPtr->Thickness = thickness;
			s_Data.m_CircleVertexBufferPtr->Fade = fade;
			s_Data.m_CircleVertexBufferPtr->EntityID = entityID;
			s_Data.m_CircleVertexBufferPtr++;
			//HZ_CORE_WARN("dRAWING Circle!!!");
		}
		s_Data.m_CircleIndexCount += 6;

		s_Data.m_Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.m_LineVertexBufferPtr->Position	= p0;
		s_Data.m_LineVertexBufferPtr->Color		= color;
		s_Data.m_LineVertexBufferPtr->EntityID	= entityID;
		s_Data.m_LineVertexBufferPtr++;
		
		s_Data.m_LineVertexBufferPtr->Position	= p1;
		s_Data.m_LineVertexBufferPtr->Color		= color;
		s_Data.m_LineVertexBufferPtr->EntityID	= entityID;
		s_Data.m_LineVertexBufferPtr++;

		s_Data.m_LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertex[4];
		for (size_t i = 0; i < 4; i++)
			lineVertex[i] = transform * s_Data.m_QuadVertexPositions[i];

		DrawLine(lineVertex[0], lineVertex[1], color);
		DrawLine(lineVertex[1], lineVertex[2], color);
		DrawLine(lineVertex[2], lineVertex[3], color);
		DrawLine(lineVertex[3], lineVertex[0], color);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, glm::vec2 size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.thickness;
	}

	void Renderer2D::SetLineWidth(float thickness)
	{
		s_Data.thickness = thickness;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, CSpriteRenderer& sprite, int entityID)
	{
		if (sprite.Texture)
			DrawQuad(transform, sprite.Color, sprite.Texture, sprite.TilingFactor, entityID);
		else
			DrawQuad(transform, sprite.Color, entityID);
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;
		s_Data.m_TextureSlotIndex = 1;
	}

	//void Renderer2D::CalculateStats(Timestep ts)
	//{
	//	s_Data.m_Stats.currentFrameTime = ts.GetMilliseconds();
	//	s_Data.m_Stats.FrameCalculateTime = s_Data.m_Stats.currentFrameTime - s_Data.m_Stats.lastFrameTime;
	//	s_Data.m_Stats.FPS = 1000.0f / s_Data.m_Stats.FrameCalculateTime;

	//	HZ_CORE_WARN("Frame Time and FPS: {0}, {1}, {2}", s_Data.m_Stats.FrameCalculateTime, s_Data.m_Stats.FPS, ts.GetMilliseconds());
	//}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.m_Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		//CalculateStats(Hazle::Renderer2D::ts);
		return s_Data.m_Stats;
	}
}