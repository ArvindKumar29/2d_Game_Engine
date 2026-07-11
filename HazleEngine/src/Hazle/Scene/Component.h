#pragma once

namespace Hazle
{
	struct CTransform
	{
		glm::mat4 Transform;

		CTransform() = default;
		CTransform(const CTransform&) = default;
		CTransform(const glm::mat4& transfrom)
			:Transform(transfrom) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () { return Transform; }
	};

	struct CMesh
	{};

	struct CSpriteRenderer
	{
		glm::vec4 Color;

		CSpriteRenderer() = default;
		CSpriteRenderer(const CSpriteRenderer&) = default;
		CSpriteRenderer(const glm::vec4& color)
			:Color(color) {}
	};

	class Component
	{
	public:
		Component();

	private:

	};
}