#pragma once

#include <glm/glm.hpp>

namespace Hazle
{
	class Camera
	{
	public:
		Camera(glm::mat4 projection)
			: m_Projection(projection) {}

		glm::mat4 GetProjection() const { return m_Projection; }

	private:
		glm::mat4 m_Projection;
	};
}