#pragma once
#include "Hazle/Core/Hazle.h"
#include "Hazle/Renderer/Camera.h"

namespace Hazle
{
	struct CTransform
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		CTransform() = default;
		CTransform(const CTransform& other) = default;
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

	struct CTag
	{
		std::string Tag;

		CTag() = default;
		CTag(const CTag&) = default;
		CTag(const std::string tag)
			:Tag(tag) {}
	};

	struct CCamera
	{
		Camera camera;
		bool Primary = true;

		CCamera() = default;
		CCamera(const CCamera&) = default;
		CCamera(const glm::mat4 projection)
			: camera(projection) {}
	
	};

	//class Component
	//{
	//public:
	//	Component();

	//private:

	//};
}