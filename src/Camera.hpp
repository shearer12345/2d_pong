#pragma once
#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSLProgram.hpp"

#include <iostream>

class Camera
{
public:
    glm::vec3 position = glm::vec3(0.0f);

    Camera(GLSLProgram* glslProgram, glm::vec3 position);
    virtual void activate() {};

protected:
    GLSLProgram* glslProgram = nullptr;
};
