#include <hzpch.h>

#include "Scene.h"

namespace Hazle
{
	Scene::Scene()
	{}
	Scene::~Scene()
	{}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<CTransform>(entt::get<CSpriteRenderer>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<CTransform, CSpriteRenderer>(entity);

			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}
}