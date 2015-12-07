#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "GLSLProgram.hpp"

class Mesh {
public:
    GLuint vertexDataBufferObject;
    GLuint vertexArrayObject;
    GLSLProgram * glslProgram;

    Mesh(GLSLProgram * glslProgram);
};
