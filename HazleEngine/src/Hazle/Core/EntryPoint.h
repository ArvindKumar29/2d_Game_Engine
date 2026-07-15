#pragma once
#include "Hazle/Core/core.h"
#include "Hazle/Core/Log.h"
#include "Hazle/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazle::Application* Hazle::createApplication();

void main(int argc, char** argv) {
	Hazle::Log::init();
	
	auto app = Hazle::createApplication();
	
	app->run();
	
	delete app;
}

#endif