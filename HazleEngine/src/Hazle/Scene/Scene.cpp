#include <hzpch.h>

#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "ScriptableEntity.h"
#include "Hazle/Renderer/Renderer2D.h"
#include "Hazle/Core/Log.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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
		glm::mat4 cameraTransform;
		auto view = m_Registry.view<CTransform, CCamera>();
		for (auto entity : view)
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
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			auto view = m_Registry.view<CTransform, CSpriteRenderer>();
			for (auto entity : view)
			{
				auto& transform = view.get<CTransform>(entity);
				auto& sprite = view.get<CSpriteRenderer>(entity);
				Hazle::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				//HZ_CORE_INFO("Drawing!!! {0} {1} {2}, {3}", transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2], glm::to_string(sprite.Color));
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto view = m_Registry.view<CCamera>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CCamera>(entity);
			if(!cameraComponent.FixedAspectRatio)
				cameraComponent.camera.SetViewportSize(width, height);
		}
	}
}