#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "Camera.hpp"

class PerspectiveCamera : public Camera {
public:

    glm::vec3 lookat;
    glm::vec3 upVector;

    GLfloat fovy = glm::radians(90.0f);
    GLfloat aspect = 1.0f;
    GLfloat zNear = 0.01f;
    GLfloat zFar = 100.0f;

    PerspectiveCamera(GLSLProgram* glslProgram,
          glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
          glm::vec3 lookat =   glm::vec3(0.0f, 0.0f, -1.0f),
          glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f)
            );

    void activate() override;
};
