#pragma once

//HAZLE
#include <glm/glm.hpp>
#include <map>
#include <iostream>
#include <string>

//FMOD
#include <fmod.hpp>


namespace Hazle
{
	struct SoundInfo
	{
		std::string name;
		float volume = 1.0f;
		float pitch = 1.0f;
		float pan = 0.0f;
		bool bLoop = false;
		bool b3D = false;
		bool bLoaded = false;
	};

	class Audio
	{
	public:
		Audio();
		~Audio();
		static void Init();
		static void Update();
		static void Shutdown();
		static void LoadSound(const std::string& path, bool is3D, bool isLooping, bool isStreaming);
		static void PlaySound(const std::string& name, const glm::vec3& position = glm::vec3(0.0f), float volume = 1.0f);
		static void StopSound(const std::string& name);
		static void SetListenerAttributes(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up);
	private:
		FMOD::System* m_StudioSystem;
		FMOD::System* m_System;
		std::map<std::string, SoundInfo> m_Sounds;
	};
}