#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "GLSLProgram.hpp"
#include "Vertex.hpp"

class Mesh {
public:

    GLSLProgram* glslProgram = nullptr;

    Mesh(GLSLProgram* glslProgram, std::vector<Vertex> vertexData =
         {
            Vertex(   glm::vec3( 0.000f,  0.500f,  0.000f),  glm::vec4(1.0f, 0.0f, 0.0f,  1.0f) ),
            Vertex(   glm::vec3(-0.433f, -0.250f,  0.000f),  glm::vec4(0.0f, 1.0f, 0.0f,  1.0f) ),
            Vertex(   glm::vec3( 0.433f, -0.250f,  0.000f),  glm::vec4(0.0f, 0.0f, 1.0f,  1.0f) ) });

    void draw(GLenum mode = GL_TRIANGLES);
    void bufferData(std::vector<Vertex> newVertexData = {});

private:
    GLuint vertexDataBufferObject = 0;
    GLuint vertexArrayObject = 0;
    std::vector<Vertex> vertexData;
    void setupVertexArrayObject();
};
