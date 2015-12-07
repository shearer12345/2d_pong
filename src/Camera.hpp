#pragma once
#include "GLSLProgram.hpp"

class Camera
{
public:

    Camera(GLSLProgram* glslProgram);
    void activate();

protected:
    GLSLProgram* glslProgram = nullptr;
};
