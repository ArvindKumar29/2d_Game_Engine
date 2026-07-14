#pragma once
#include "SceneCamera.h"

namespace Hazle
{
	class ScriptableEntity;
	struct CTransform
	{
		glm::mat4 Transform{ 1.0f };

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
		glm::vec4 Color = {0.0f, 1.0f, 1.0f, 1.0f};
		//Ref<MaterialInstance> m_MaterialInstance;

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
		SceneCamera camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CCamera() = default;
		CCamera(const CCamera&) = default;
		CCamera(const glm::mat4 projection)
			: camera() {}
	
	};

	struct CNativeScript
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(CNativeScript*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](CNativeScript* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}