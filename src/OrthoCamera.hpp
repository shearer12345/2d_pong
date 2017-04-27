#pragma once
#include "Camera.hpp"

class OrthoCamera : public Camera {
public:
  glm::vec2 size = glm::vec2(2.0f, 2.0f);

  OrthoCamera(GLSLProgram *glslProgram, glm::vec3 position = glm::vec3(0.0f),
              glm::vec2 size = glm::vec2(2.0f));
  void activate() override;
};
