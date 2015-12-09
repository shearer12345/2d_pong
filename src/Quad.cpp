#include "Quad.hpp"

const float low = -1.0f;
const float high = 1.0f;

Quad::Quad(GLSLProgram* glslProgram) :
              Mesh::Mesh(glslProgram, std::vector<Vertex> {
    Vertex(   glm::vec3(  low, high,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(0.0f, 1.0f) ),
    Vertex(   glm::vec3(  low,  low,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(0.0f, 0.0f) ),
    Vertex(   glm::vec3( high, high,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(1.0f, 1.0f) ),
    Vertex(   glm::vec3( high, high,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(1.0f, 1.0f) ),
    Vertex(   glm::vec3(  low,  low,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(0.0f, 0.0f) ),
    Vertex(   glm::vec3( high,  low,  0.0f),  glm::vec4(1.0f, 1.0f, 1.0f,  1.0f),  glm::vec2(1.0f, 0.0f) ) })
{}
