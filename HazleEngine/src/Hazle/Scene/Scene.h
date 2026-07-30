#pragma once
#include <entt.hpp>
#include "Hazle/Core/Timestep.h"
#include "Hazle/Core/UUID.h"
#include "SceneCamera.h"
#include "Hazle/Renderer/EditorCamera.h"

class b2World;

namespace Hazle
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... T>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<T...>();
		}

		static Ref<Scene> Copy(Ref<Scene> other);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight;

		b2World* m_PhysicsWorld = nullptr;


		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}