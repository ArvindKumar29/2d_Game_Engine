#pragma once

#include "entt.hpp"
#include "Scene.h"
#include "Component.h"
#include "Hazle/Core/core.h"
#include "Hazle/Core/UUID.h"

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
		T& AddOrReplaceComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityManager, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
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
		operator entt::entity() const {	return m_EntityManager; }
		operator bool() const { return m_EntityManager != entt::null; }
		bool operator==(const Entity& other) const { return m_EntityManager == other.m_EntityManager && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return *this == other; }

		UUID GetUUID() { return getComponent<CID>().ID; }
		const std::string&  GetName() { return getComponent<CTag>().Tag; }

	private:
		entt::entity m_EntityManager{ entt::null };
		Scene* m_Scene = nullptr;
		friend class SceneHierarchyPanel;
	};
}