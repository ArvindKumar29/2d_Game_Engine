#include <hzpch.h>

#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "ScriptableEntity.h"
#include "Hazle/Renderer/Renderer2D.h"
namespace Hazle
{
	Scene::Scene()
	{}

	Scene::~Scene()
	{}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<CTag>();
		auto& tag = entity.getComponent<CTag>();
		tag.Tag = name.empty() ? "Entity" : name;
		HZ_CORE_TRACE("Entity added: {}", tag.Tag);
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
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

		// Render2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.group<CTransform, CCamera>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<CTransform, CCamera>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto view = m_Registry.view<CTransform, CSpriteRenderer>();
			view.each([](auto entity, auto& transform, auto& sprite)
				{
					//transform.Transform[3][2] = -1.0f;
					Hazle::Renderer2D::DrawQuad(transform.Transform, sprite.Color);
				});
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		//m_ViewportWidth = width;
		//m_ViewportHeight = height;

		//auto view = m_Registry.view<CCamera>();
		//for (auto entity : view)
		//{
		//	auto& cameraComponent = view.get<CCamera>(entity);
		//	if (!cameraComponent.FixedAspectRatio)
		//		cameraComponent.Camera.SetViewportSize(width, height);
		//}
	}
}