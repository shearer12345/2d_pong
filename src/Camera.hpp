#pragma once

#include "GLSLProgram.hpp"
#include <glm/glm.hpp>

class Camera {
public:
  glm::vec3 position = glm::vec3(0.0f);

  Camera(GLSLProgram *glslProgram, glm::vec3 position);
  virtual void activate(){};

protected:
  GLSLProgram *glslProgram = nullptr;
};
