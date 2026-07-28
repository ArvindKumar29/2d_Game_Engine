#include <hzpch.h>
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node ecnode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node ecnode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node ecnode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}


namespace Hazle
{
	static CRigidBody2D::BodyType StringToRigidBodyType(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")
			return CRigidBody2D::BodyType::Static;

		if (bodyTypeString == "Dynamic")
			return CRigidBody2D::BodyType::Dynamic;
		
		if (bodyTypeString == "Kinamatic")
			return CRigidBody2D::BodyType::Kinamatic;
		
		HZ_CORE_ASSERT(false, "Unknown Body Type String!!!");
		return CRigidBody2D::BodyType::Static;

	}
	
	static std::string RigidBodyTypeToString(CRigidBody2D::BodyType bodyType)
	{
		switch (bodyType)
		{
			case CRigidBody2D::BodyType::Static:	return "Static";
			case CRigidBody2D::BodyType::Dynamic:	return "Dynamic";
			case CRigidBody2D::BodyType::Kinamatic: return "Kinamatic";
		}
		HZ_CORE_ASSERT(false, "Unknown Body Type!!!");
		return {};
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{}
	
	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;									// Entity
		out << YAML::Key << "Entity" << YAML::Value << "1234567890"; // EntityID goes here///

		if (entity.hasComponent<CTag>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;								// TagComponent

			auto& tag = entity.getComponent<CTag>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;								// TagComponent 
		}

		if (entity.hasComponent<CTransform>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;								// Transform Component

			auto& tc = entity.getComponent<CTransform>();
			out << YAML::Key << "Translation" << YAML::Flow << YAML::BeginSeq << tc.Translation.x << tc.Translation.y << tc.Translation.z << YAML::EndSeq;
			out << YAML::Key << "Rotation" << YAML::Flow << YAML::BeginSeq << tc.Rotation.x << tc.Rotation.y << tc.Rotation.z << YAML::EndSeq;
			out << YAML::Key << "Scale" << YAML::Flow << YAML::BeginSeq << tc.Scale.x << tc.Scale.y << tc.Scale.z << YAML::EndSeq;

			out << YAML::EndMap;
		}

		if (entity.hasComponent<CCamera>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;								// Camera Component

			auto& cameraComponent = entity.getComponent<CCamera>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;								// Camera Type
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "Perspective FOV" << YAML::Value << camera.GetPerspectiveVerticleFOV();
			out << YAML::Key << "Perspective Near" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "Perspective Far" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "Orthographic Size" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "Orthographic Near" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "Orthographic Far" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;								// Camera Type


			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::EndMap;								// Camera Component
		}

		if (entity.hasComponent<CSpriteRenderer>())
		{
			out << YAML::Key << "SpriteRendrerComponent";
			out << YAML::BeginMap;								// Sprite Renderer Component

			auto& sprite = entity.getComponent<CSpriteRenderer>();
			out << YAML::Key << "Color" << YAML::Flow << YAML::BeginSeq
				<< sprite.Color.x << sprite.Color.y << sprite.Color.z << sprite.Color.a << YAML::EndSeq;

			out << YAML::EndMap;								// Sprite Renderer Component
		}

		if (entity.hasComponent<CRigidBody2D>())
		{
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::BeginMap;								// RigidBodyComponent

			auto& rb2d = entity.getComponent<CRigidBody2D>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBodyTypeToString(rb2d.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;

			out << YAML::EndMap;								// RigidBodyComponent
		}

		if (entity.hasComponent<CBoxCollider2D>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;								// BoxCollider2DComponent

			auto& bc2d = entity.getComponent<CBoxCollider2D>();
			out << YAML::Key << "Offset" << YAML::Flow << YAML::BeginSeq << bc2d.Offset.x << bc2d.Offset.y << YAML::EndSeq;
			out << YAML::Key << "Size" << YAML::Flow << YAML::BeginSeq << bc2d.Size.x << bc2d.Size.y << YAML::EndSeq;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;

			out << YAML::EndMap;								// BoxCollider2DComponent
		}
		out << YAML::EndMap;											// Entity
	}

	void SceneSerializer::Serialize(const std::string & filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene"	<< YAML::Value << "Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		//ID 0 TRAP... FIRST ENTITY NEVER GETS SERIALIZED DUE TO ID 0
		//m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID) 
		//	{
		//		Entity entity = { entityID, m_Scene.get() };
		//		if (!entity)
		//			return;
		//		SerializeEntity(out, entity);
		//	});

		auto view = m_Scene->m_Registry.view<CTag>();
		for (auto entityID : view)
		{
			Entity entity = { entityID, m_Scene.get() };

			// Yahan 'if (!entity)' lagane ki zaroorat hi nahi hai, 
			// kyunki EnTT wahi entities dega jo legitimately zinda hain.
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	
	void SceneSerializer::SerializeRuntine(const std::string & filepath)
	{
		// Not Implemented
		HZ_CORE_ASSERT(false);
	}
	
	bool SceneSerializer::DeSerialize(const std::string & filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		auto sceneName = data["Scene"];
		HZ_CORE_ASSERT("Deserializing Scene '{0}'", sceneName);;

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint32_t uuid = entity["Entity"].as<uint32_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();
			
				HZ_CORE_ASSERT("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					if (!deserializedEntity.hasComponent<CTransform>())
						deserializedEntity.AddComponent<CTransform>();
					auto& tc = deserializedEntity.getComponent<CTransform>();
					tc.Translation	= transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation		= transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale		= transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CCamera>();
					auto& cameraProps = cameraComponent["Camera"];

					cc.camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.SetPerspectiveVerticleFOV(cameraProps["Perspective FOV"].as<float>());
					cc.camera.SetPerspectiveNearClip(cameraProps["Perspective Near"].as<float>());
					cc.camera.SetPerspectiveFarClip(cameraProps["Perspective Far"].as<float>());

					cc.camera.SetOrthographicSize(cameraProps["Orthographic Size"].as<float>());
					cc.camera.SetOrthographicNearClip(cameraProps["Orthographic Near"].as<float>());
					cc.camera.SetOrthographicFarClip(cameraProps["Orthographic Far"].as<float>());

					cc.Primary			= cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendrerComponent"];
				if (spriteRendererComponent)
				{
					auto& sprite = deserializedEntity.AddComponent<CSpriteRenderer>();
					sprite.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
				
				auto RigidBodyComponent = entity["RigidBodyComponent"];
				if (RigidBodyComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<CRigidBody2D>();
					rb2d.Type = StringToRigidBodyType(RigidBodyComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = RigidBodyComponent["FixedRotation"].as<bool>();
				}
				
				auto BoxColliderComponent = entity["BoxCollider2DComponent"];
				if (BoxColliderComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<CBoxCollider2D>();
					bc2d.Offset = BoxColliderComponent["Offset"].as<glm::vec2>();
					bc2d.Size = BoxColliderComponent["Size"].as<glm::vec2>();
				
					bc2d.Density = BoxColliderComponent["Density"].as<float>();
					bc2d.Friction = BoxColliderComponent["Friction"].as<float>();
					bc2d.Restitution = BoxColliderComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = BoxColliderComponent["RestitutionThreshold"].as<float>();
				
				}

			}
		}
		return true;
	}
	
	bool SceneSerializer::DeSerializeRuntine(const std::string & filepath)
	{
		// Not Implemented
		HZ_CORE_ASSERT(false);
		return false;
	}
}