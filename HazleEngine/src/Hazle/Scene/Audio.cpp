#include "hzpch.h"
#include "Audio.h"
//#include <fmod.hpp>

namespace Hazle {

	namespace {
		FMOD::System* s_System = nullptr;
		std::map<std::string, FMOD::Sound*> s_Sounds;
		std::map<std::string, FMOD::Channel*> s_Channels;
		std::map<std::string, SoundInfo> s_SoundInfo;
	}

	void Audio::Init()
	{
		FMOD::System_Create(&s_System);
		s_System->init(512, FMOD_INIT_NORMAL, nullptr);
	}

	void Audio::Update()
	{
		if (s_System) s_System->update();
	}

	void Audio::Shutdown()
	{
		if (s_System)
		{
			for (auto const& [name, sound] : s_Sounds) {
				sound->release();
			}
			s_Sounds.clear();
			s_Channels.clear();
			s_SoundInfo.clear();

			s_System->release();
			s_System = nullptr;
		}
	}

	void Audio::LoadSound(const std::string& path, bool is3D, bool isLooping, bool isStreaming)
	{
		if (s_Sounds.find(path) != s_Sounds.end()) return;

		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= is3D ? FMOD_3D : FMOD_2D;
		mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= isStreaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		FMOD::Sound* sound = nullptr;
		s_System->createSound(path.c_str(), mode, nullptr, &sound);

		if (sound)
		{
			s_Sounds[path] = sound;

			SoundInfo info;
			info.name = path;
			info.b3D = is3D;
			info.bLoop = isLooping;
			info.bLoaded = true;
			s_SoundInfo[path] = info;
		}
	}

	void Audio::PlaySound(const std::string& name, const glm::vec3& position, float volume)
	{
		if (s_Sounds.find(name) == s_Sounds.end())
		{
			LoadSound(name, false, false, false);
		}

		FMOD::Sound* sound = s_Sounds[name];
		if (!sound) return;

		FMOD::Channel* channel = nullptr;
		s_System->playSound(sound, nullptr, true, &channel);

		if (channel)
		{
			if (s_SoundInfo[name].b3D)
			{
				FMOD_VECTOR pos = { position.x, position.y, position.z };
				FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
				channel->set3DAttributes(&pos, &vel);
			}

			channel->setVolume(volume);
			channel->setPaused(false);
			s_Channels[name] = channel;
		}
	}

	void Audio::StopSound(const std::string& name)
	{
		auto it = s_Channels.find(name);
		if (it != s_Channels.end())
		{
			it->second->stop();
			s_Channels.erase(it);
		}
	}

	void Audio::SetListenerAttributes(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up)
	{
		if (s_System)
		{
			FMOD_VECTOR pos = { position.x, position.y, position.z };
			FMOD_VECTOR fwd = { forward.x, forward.y, forward.z };
			FMOD_VECTOR upVec = { up.x, up.y, up.z };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			s_System->set3DListenerAttributes(0, &pos, &vel, &fwd, &upVec);
		}
	}
}