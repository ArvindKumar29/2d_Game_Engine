#include <hzpch.h>

#include "UUID.h"
#include <random>

#include <unordered_map>

// we use unordered_map to store the UUIDs and their corresponding names for debugging purposes. 
// This allows us to easily look up the name of an entity given its UUID, which can be helpful for logging and debugging.
static std::unordered_map<Hazle::UUID, std::string> m_Map;

static void AddToMap()
{
	m_Map[Hazle::UUID()] = "Entity";
}

namespace Hazle
{
	static std::random_device s_RandomDevice;					// Random device used to seed the random number generator.
	static std::mt19937_64 s_Engine(s_RandomDevice());			// Mersenne Twister random number generator engine.
	static std::uniform_int_distribution s_UniformDistribution;	// Uniform distribution used to generate random UUIDs.

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
