#include <hzpch.h>
#include "Entity.h"
#include "Scene.h"

namespace Hazle
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityManager(handle), m_Scene(scene)
	{

	}
	Entity::~Entity()
	{}
}