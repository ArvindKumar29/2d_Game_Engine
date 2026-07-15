#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazle
{
	class ScriptableEntity;

	struct CTransform
	{
		//glm::mat4 Transform{ 1.0f };
		glm::vec3 Translation	= { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation		= { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale			= { 1.0f, 1.0f, 1.0f };
		glm::mat4 Tarnsform;

		CTransform() = default;
		CTransform(const CTransform& other) = default;
		CTransform(const glm::vec3& translation)
			:Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.x, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.x, { 0, 0, 1 });

			return  glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct CSpriteRenderer
	{
		glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
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
		bool Primary = false;
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