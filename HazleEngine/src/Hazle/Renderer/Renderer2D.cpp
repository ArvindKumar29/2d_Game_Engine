#include "hzpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Hazle/Core/Log.h"

namespace Hazle
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 1000000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: RenderCaps

		Ref<VertexArray> m_QuadVA;
		Ref<VertexBuffer> m_QuadBuffer;
		Ref<Shader> m_TextureShader;
		Ref<Texture2D> m_WhiteTexture;

		uint32_t m_QuadIndexCount = 0;
		QuadVertex* m_QuadVertexBufferBase = nullptr;
		QuadVertex* m_QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> m_TextureSlots;
		uint32_t m_TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 m_QuadVertexPositions[4];

		Renderer2D::Statistics m_Stats;
	};


	Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_Data.m_QuadVA = VertexArray::Create();

		s_Data.m_QuadBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

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

		s_Data.m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xffffffff;
		s_Data.m_WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));

		BufferLayout layout2 = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"}
		};

		s_Data.m_QuadBuffer->SetLayout(layout2);
		s_Data.m_QuadVA->AddVertexBuffer(s_Data.m_QuadBuffer);
		s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.m_TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
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
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_VP", viewProj);
		s_Data.m_WhiteTexture->Bind(0);
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_TextureSlotIndex = 1;

		//HZ_CORE_WARN("SCENE STARTED WITH CAMERA!!!");
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_VP", camera.GetVPMatrix());
		s_Data.m_WhiteTexture->Bind(0);
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_TextureSlotIndex = 1; 
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)s_Data.m_QuadVertexBufferPtr - (uint8_t*)s_Data.m_QuadVertexBufferBase;
		s_Data.m_QuadBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();
		for (uint32_t i = 0; i < s_Data.m_TextureSlotIndex; i++)
			s_Data.m_TextureSlots[i]->Bind(i);

		if (s_Data.m_QuadIndexCount == 0)
			return;
		RenderCommand::DrawIndexed(s_Data.m_QuadVA, s_Data.m_QuadIndexCount);
		s_Data.m_Stats.DrawCalls++;
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
		HZ_PROFILE_FUNCTION();

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
		HZ_PROFILE_FUNCTION();

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
		HZ_PROFILE_FUNCTION();

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
			textureIndex = s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[0];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[1];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[2];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[3];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

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
			s_Data.m_QuadVertexBufferPtr++;
		}
		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;

		//HZ_CORE_WARN("dRAWING QUAD!!!");

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D> texture, const float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

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
			textureIndex = s_Data.m_TextureSlotIndex;
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

		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[0];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[1];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[2];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;
		s_Data.m_QuadVertexBufferPtr->Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->Color = color;
		s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoord[3];
		s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.QuadCount++;

	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;
		s_Data.m_TextureSlotIndex = 1;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.m_Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.m_Stats;
	}
}