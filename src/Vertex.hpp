#pragma once

#include <glm/glm.hpp>

class Vertex
{
public:
	glm::vec3 position;
	glm::vec4 color;

	Vertex();

	Vertex(glm::vec3 pos, glm::vec4 col)
	{
		position = pos;
		color = col;
	};
};
