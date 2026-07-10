#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "core.h"

namespace Hazle 
{
	class Input 
	{
	public:
		static bool IsKeyPressed(int keycode);
		
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX(); 
		static float GetMouseY(); 
	};
}