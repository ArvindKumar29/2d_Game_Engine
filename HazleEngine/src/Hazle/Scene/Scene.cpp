#include <hzpch.h>

#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "ScriptableEntity.h"
#include "Hazle/Renderer/Renderer2D.h"
#include "Hazle/Core/Log.h"

// BOX2D PHYSICS
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Hazle
{
	static b2BodyType RigidBody2DTypeToBox2DBodyType(CRigidBody2D::BodyType bodyType)
	{
		switch (bodyType)
		{
			case CRigidBody2D::BodyType::Static:	return b2BodyType::b2_staticBody;
			case CRigidBody2D::BodyType::Dynamic:	return b2BodyType::b2_dynamicBody;
			case CRigidBody2D::BodyType::Kinamatic: return b2BodyType::b2_kinematicBody;
		}
		HZ_CORE_ASSERT(false, "Unknown body type!!!");
		return b2_staticBody;
	}

	Scene::Scene()
	{}

	Scene::~Scene()
	{}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity  entity = CreateEntityWithUUID(UUID(), name);
		return entity;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddOrReplaceComponent<CID>(uuid);
		entity.AddOrReplaceComponent<CTag>();
		auto& tag = entity.getComponent<CTag>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({0.0f, -9.8f});
		auto view = m_Registry.view<CRigidBody2D>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.getComponent<CTransform>();
			auto& rb2d = entity.getComponent<CRigidBody2D>();

			b2BodyDef bodyDef;
			bodyDef.type = RigidBody2DTypeToBox2DBodyType(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.hasComponent<CBoxCollider2D>())
			{
				auto& bc2d = entity.getComponent<CBoxCollider2D>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape				= &boxShape;
				fixtureDef.density				= bc2d.Density;
				fixtureDef.friction				= bc2d.Friction;
				fixtureDef.restitution			= bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}

			if (entity.hasComponent<CCircleCollider2D>())
			{
				auto& cc2d = entity.getComponent<CCircleCollider2D>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape				= &circleShape;
				fixtureDef.density				= cc2d.Density;
				fixtureDef.friction				= cc2d.Friction;
				fixtureDef.restitution			= cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
		}

	}
	
	
	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		{

			auto view = m_Registry.view<CTransform, CSpriteRenderer>();
			for (auto entity : view)
			{
				auto& transform = view.get<CTransform>(entity);
				auto& sprite = view.get<CSpriteRenderer>(entity);
				//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity); // draw actual sprites
				//Renderer2D::DrawRect(transform.GetTransform(), {1.0f, 1.0f, 0.0f, 1.0f}, (int)entity); // draw wireframe of the entities
			}
		}

		{
			auto view = m_Registry.view<CTransform, CCircleRenderer>();
			for (auto entity : view)
			{
				auto& transform = view.get<CTransform>(entity);
				auto& circle = view.get<CCircleRenderer>(entity);
				//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
		}

		// USE FOR DUBUG ONLY
		//Renderer2D::DrawLine(glm::vec3(0.0f), glm::vec3(2.0f, 2.0f, 0.0f), { 0.0f, 1.0f, 1.0f, 1.0f });
		//Renderer2D::DrawRect(glm::vec3(0.0f), {5.0f, 5.0f},  { 1.0f, 0.0f, 1.0f, 1.0f });

		Renderer2D::EndScene();
	}



	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<CNativeScript>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}

		//Physics (Scripts->Physics->Render everyone is linked and uses updated before it...)
		if(m_PhysicsWorld)
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			//Retrieve Transform from Box2D
			auto view = m_Registry.view<CRigidBody2D>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.getComponent<CTransform>();
				auto& rb2d = entity.getComponent<CRigidBody2D>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z	= body->GetAngle();
			}
		}


		// Render2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		auto& view = m_Registry.view<CTransform, CCamera>();
		for (auto& entity : view)
		{
			auto& transform = view.get<CTransform>(entity);
			auto& camera = view.get<CCamera>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.camera;
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			//HZ_CORE_INFO("PROJECTION: {0}", glm::to_string(mainCamera->GetProjection()));
			//HZ_CORE_INFO("VIEW: {0}", glm::to_string(glm::inverse(*cameraTransform)));

			//Draw sprites/Quads
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			{
				auto& view = m_Registry.view<CTransform, CSpriteRenderer>();
				for (auto& entity : view)
				{
					auto& transform = view.get<CTransform>(entity);
					auto& sprite = view.get<CSpriteRenderer>(entity);
					if (sprite.Texture)
						Hazle::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, sprite.Texture);
					else
						Hazle::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);

					//HZ_CORE_INFO("Drawing!!! {0} {1} {2}, {3}", transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2], glm::to_string(sprite.Color));
				}
			}

			// Draw Circles
			{
				auto& view = m_Registry.view<CTransform, CCircleRenderer>();
				for (auto& entity : view)
				{
					auto& transform = view.get<CTransform>(entity);
					auto& circle = view.get<CCircleRenderer>(entity);
					
					Hazle::Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);

					//HZ_CORE_INFO("Drawing!!! {0} {1} {2}, {3}", transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2], glm::to_string(sprite.Color));
				}
			}
			

			// USE FOR DEBUG ONLY
			//Renderer2D::DrawLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 0.0f), { 0.0f, 1.0f, 1.0f, 1.0f });
			//Renderer2D::DrawRect(glm::vec3(2.0f), glm::vec2(5.0f), { 0.0f, 1.0f, 1.0f, 1.0f });

			Renderer2D::EndScene();
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CCamera>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CCamera>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto& view = m_Registry.view<CCamera>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CCamera>(entity);
			if(!cameraComponent.FixedAspectRatio)
				cameraComponent.camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	static void CopyComponent(entt::registry& dst, entt::registry& src, std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<T>();
		for (auto e : view)
		{
			UUID uuid = src.get<CID>(e).ID;
			entt::entity dstEnttID = enttMap.at(uuid);
			auto& component = src.get<T>(e);

			dst.emplace_or_replace<T>(dstEnttID, component);
		}
	}

	template<typename T>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.hasComponent<T>())
			dst.AddOrReplaceComponent<T>(src.getComponent<T>());
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);


		//Copy Components except tag and ID
		CopyComponentIfExists<CTransform>		(newEntity, entity);
		CopyComponentIfExists<CCamera>			(newEntity, entity);
		CopyComponentIfExists<CSpriteRenderer>	(newEntity, entity);
		CopyComponentIfExists<CCircleRenderer>	(newEntity, entity);
		CopyComponentIfExists<CNativeScript>	(newEntity, entity);
		CopyComponentIfExists<CRigidBody2D>		(newEntity, entity);
		CopyComponentIfExists<CBoxCollider2D>	(newEntity, entity);
		CopyComponentIfExists<CCircleCollider2D>(newEntity, entity);
	}



	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->m_ViewportWidth  = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcSceneReg = other->m_Registry;
		auto& dstSceneReg = newScene->m_Registry;

		//Create Entities in newScene and copy tag and ID components
		auto idView = srcSceneReg.view<CID>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneReg.get<CID>(e).ID;
			const auto& name = srcSceneReg.get<CTag>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity;
		}

		// Copy components except ID and Tag
		CopyComponent<CTransform>		(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CCamera>			(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CSpriteRenderer>	(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CCircleRenderer>	(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CNativeScript>	(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CRigidBody2D>		(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CBoxCollider2D>	(dstSceneReg, srcSceneReg, enttMap);
		CopyComponent<CCircleCollider2D>(dstSceneReg, srcSceneReg, enttMap);

		return newScene;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0, "Unknown Component Added");
	}
	
	template<>
	void Scene::OnComponentAdded<CID>(Entity entity, CID& component)
	{}
	
	template<>
	void Scene::OnComponentAdded<CTransform>(Entity entity, CTransform& component)
	{}
	
	template<>
	void Scene::OnComponentAdded<CCamera>(Entity entity, CCamera& component)
	{
		component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CSpriteRenderer>(Entity entity, CSpriteRenderer& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CCircleRenderer>(Entity entity, CCircleRenderer& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CTag>(Entity entity, CTag& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CNativeScript>(Entity entity, CNativeScript& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CRigidBody2D>(Entity entity, CRigidBody2D& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CBoxCollider2D>(Entity entity, CBoxCollider2D& component)
	{}
	
	template<>
	void Hazle::Scene::OnComponentAdded<CCircleCollider2D>(Entity entity, CCircleCollider2D& component)
	{}
	
}