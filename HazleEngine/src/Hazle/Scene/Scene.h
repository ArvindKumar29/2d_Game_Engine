#pragma once
#include <entt.hpp>
#include "Hazle/Core/Timestep.h"

namespace Hazle
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}