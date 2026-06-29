#pragma once
#include "Hazle/Core/core.h"
#include "Hazle/Core/Log.h"
#include "Hazle/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazle::Application* Hazle::createApplication();

void main(int argc, char** argv) {
	Hazle::Log::init();
	
	HZ_PROFILE_BEGIN_SESSION("Startup", "HazleProfile-Startup.json");
	auto app = Hazle::createApplication();
	HZ_PROFILE_END_SESSION();
	
	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazleProfile-Runtime.json");
	app->run();
	HZ_PROFILE_END_SESSION();
	
	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazleProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif