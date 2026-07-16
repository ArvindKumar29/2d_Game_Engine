#pragma once

#include "Scene.h"

namespace Hazle
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);			// Text Serialization
		void SerializeRuntine(const std::string& filepath);		// Binary Serialization
	
		bool DeSerialize(const std::string& filepath);			// Text Serialization
		bool DeSerializeRuntine(const std::string& filepath);	// Binary Serialization

	private:
		Ref<Scene> m_Scene;

	};
}