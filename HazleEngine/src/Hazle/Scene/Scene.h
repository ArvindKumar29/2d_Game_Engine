#pragma once
#include <entt.hpp>
#include "glm/glm.hpp"
#include "Hazle/Core/Timestep.h"

namespace Hazle
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		//TEMP
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}