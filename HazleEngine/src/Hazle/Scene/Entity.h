#pragma once

#include "entt.hpp"
#include "Scene.h"
#include "Hazle/Core/core.h"

namespace Hazle
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity();

		template<typename T>
		bool hasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityManager);
		}

		template<typename T ,typename... Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityManager, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			HZ_CORE_ASSERT(hasComponent<T>(), "Entity does not has component!");
			return m_Scene->m_Registry.get<T>(m_EntityManager);
		}

		template<typename T>
		void removeComponent()
		{
			HZ_CORE_ASSERT(hasComponent<T>(), "Entity does not has component!");
			m_Scene->m_Registry.remove<T>(m_EntityManager);
		}

		operator uint32_t() const {	return (uint32_t)m_EntityManager; }
		bool operator==(const Entity& other) const { return m_EntityManager == other.m_EntityManager && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return *this == other; }

	private:
		entt::entity m_EntityManager{ entt::null };
		Scene* m_Scene = nullptr;
	};
}