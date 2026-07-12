#include <hzpch.h>

#include "Scene.h"
#include "Entity.h"

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
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.group<CTransform, CCamera>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<CTransform, CCamera>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if(mainCamera)
		{
			HZ_CORE_INFO("Running rendering through scene");
			//Hazle::OrthographicCamera bypassCamera(-16.0f, 16.0f, -9.0f, 9.0f);
			//Hazle::Renderer2D::BeginScene(bypassCamera);
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
}