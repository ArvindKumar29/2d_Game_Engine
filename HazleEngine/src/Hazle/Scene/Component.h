#pragma once
#include "SceneCamera.h"
#include "Hazle/Renderer/Texture.h"
#include "Hazle/Core/UUID.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazle
{
	struct CID
	{
		UUID ID;

		CID() = default;
		CID(const CID& other) = default;

	};

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
			glm::mat4 rotation = glm::mat4_cast(glm::quat(Rotation));

			return  glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct CSpriteRenderer
	{
		glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

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

	class ScriptableEntity;
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

	//Physics related stuff here
	struct CRigidBody2D
	{
		enum class BodyType { Static, Kinamatic, Dynamic  };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		void* RuntimeBody = nullptr;

		CRigidBody2D() = default;
		CRigidBody2D( const CRigidBody2D& ) = default;

	};
	
	
	struct CBoxCollider2D
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		CBoxCollider2D() = default;
		CBoxCollider2D( const CBoxCollider2D& ) = default;

	};
}