#pragma once
#include "core.h"
#include "spdlog/spdlog.h"
//#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"



namespace Hazle{
	class HAZLE_API Log
	{
	public:
		static void init();
		inline static Hazle::Ref<spdlog::logger> getCoreLogger() { return s_CoreLogger; }
		inline static Hazle::Ref<spdlog::logger> getClientLogger() { return s_ClientLogger; }

	private:
		static Hazle::Ref<spdlog::logger> s_CoreLogger;
		static Hazle::Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define HZ_CORE_TRACE(...)		::Hazle::Log::getCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)		::Hazle::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)		::Hazle::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)		::Hazle::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_CRITICAL(...)	::Hazle::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HZ_TRACE(...)			::Hazle::Log::getClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)			::Hazle::Log::getClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)			::Hazle::Log::getClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)			::Hazle::Log::getClientLogger()->error(__VA_ARGS__)
#define HZ_CRITICAL(...)		::Hazle::Log::getClientLogger()->critical(__VA_ARGS__)
