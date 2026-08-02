#pragma once

namespace Hazle
{
	// Timestep class is used to represent the time between frames in seconds, 
	// it takes time from GLFW and converts it to seconds, 
	// and then we can use it to update our game logic based on the time between frames.
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}