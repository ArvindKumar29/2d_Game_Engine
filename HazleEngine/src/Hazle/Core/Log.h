#pragma once
#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"



namespace Hazle{
	class Log
	{
	public:
		static void init();
		inline static Ref<spdlog::logger> getCoreLogger() { return s_CoreLogger; } // Logger for Hazle Engine
		inline static Ref<spdlog::logger> getClientLogger() { return s_ClientLogger; } // Logger for Client Application and games

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
// Specific to the engine's internal logging and used by engine developers
#define HZ_CORE_TRACE(...)		::Hazle::Log::getCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)		::Hazle::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)		::Hazle::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)		::Hazle::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_CRITICAL(...)	::Hazle::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
// Specific to the game/application code that uses the engine and used by game developers
#define HZ_TRACE(...)			::Hazle::Log::getClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)			::Hazle::Log::getClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)			::Hazle::Log::getClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)			::Hazle::Log::getClientLogger()->error(__VA_ARGS__)
#define HZ_CRITICAL(...)		::Hazle::Log::getClientLogger()->critical(__VA_ARGS__)
