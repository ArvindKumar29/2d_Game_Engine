#pragma once
#include "core.h"

namespace Hazle
{
	class HAZLE_API Application
	{
		public:
			Application();
			virtual ~Application();
			void run();
	};

	// To be defined in client
	Application* createApplication();
}

