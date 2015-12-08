#pragma once
#include "OrthoCamera.hpp"

class TrackingOrthoCamera : public OrthoCamera
{
public:
    glm::vec3* target = nullptr;

    TrackingOrthoCamera(GLSLProgram* glslProgram, glm::vec3* target, glm::vec2 size = glm::vec2(2.0f));
    void activate() override;
};
