#pragma once

#include <glm/glm.hpp>

namespace Hazle::Math
{
	
	bool DecomposeTransform(glm::mat4& transform, 
		glm::vec3& outTransloation, 
		glm::vec3& outRotation, 
		glm::vec3& outScale);
	
}