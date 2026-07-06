#pragma once

#include "Texture.h"

namespace Hazle
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		~SubTexture2D();

		const glm::vec2* GetTexCoords() { return m_TextureCoords; }
		const Ref<Texture2D>& GetTexture() { return m_Texture; }
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};
}