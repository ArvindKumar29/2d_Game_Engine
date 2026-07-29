#include <hzpch.h>

#include "UUID.h"
#include <random>

#include <unordered_map>

static std::unordered_map<Hazle::UUID, std::string> m_Map;

static void AddToMap()
{
	m_Map[Hazle::UUID()] = "Entity";
}

namespace Hazle
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution s_UniformDistribution;

	// We don't use this because this is local and cause common UUIDs in server...
	//static std::unordered_set<uint64_t> s_IDs;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}
