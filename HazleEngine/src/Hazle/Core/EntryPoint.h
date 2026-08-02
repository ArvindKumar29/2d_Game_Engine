#pragma once
#include "Hazle/Core/core.h"
#include "Hazle/Core/Log.h"
#include "Hazle/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazle::Application* Hazle::createApplication(); // Forward Declaration to create the application in the client 
													   // and will be called by the entry point of hazle engine to create the application in the client

	void main(int argc, char** argv) { // Entry Point for Windows
		Hazle::Log::init(); // Initialize the logging system
			
		auto app = Hazle::createApplication(); // Create the application using the forward declared function
			
		app->run(); // Run the application
			
		delete app; // Delete the application to free memory
	}

#endif