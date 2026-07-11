#pragma once
#include <entt.hpp>
#include "glm/glm.hpp"
#include "Hazle/Core/Hazle.h"

namespace Hazle
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();
		//TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};
}