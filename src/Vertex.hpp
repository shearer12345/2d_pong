#pragma once

#include <glm/glm.hpp>

class Vertex {
public:
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 uv;

  Vertex();

  Vertex(glm::vec3 position, glm::vec4 color, glm::vec2 uv)
      : position(position), color(color), uv(uv){};
};
