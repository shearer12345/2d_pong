#pragma once
#include "OrthoCamera.hpp"
#include "Model.hpp"

class TrackingOrthoCamera : public OrthoCamera
{
public:
    Model* target = nullptr;

    TrackingOrthoCamera(GLSLProgram* glslProgram, Model* target, glm::vec2 size = glm::vec2(2.0f));
    void activate() override;
};
