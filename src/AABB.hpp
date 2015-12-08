#pragma once

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include "Mesh.hpp"

class AABB
{
public:
	glm::vec3 min;
	glm::vec3 max;

	AABB(glm::vec3 min, glm::vec3 max);
	AABB(glm::vec4 min, glm::vec4 max);
	AABB(Mesh * mesh);
};
