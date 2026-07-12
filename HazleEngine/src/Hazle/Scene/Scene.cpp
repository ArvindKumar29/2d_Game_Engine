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
		auto group = m_Registry.group<CTransform>(entt::get<CSpriteRenderer>);
		for (auto entity : group)
		{
			HZ_CORE_WARN("ecs live to draw entity");
			auto& [transform, sprite] = group.get<CTransform, CSpriteRenderer>(entity);
			HZ_CORE_WARN("Position X:{0}, Y:{1}, Z:{2}", transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2]);

			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}
}