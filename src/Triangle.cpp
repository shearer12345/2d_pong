#include "Triangle.hpp"

Triangle::Triangle(GLSLProgram *glslProgram)
    : Mesh::Mesh(glslProgram,
                 std::vector<Vertex>{Vertex(glm::vec3(0.000f, 0.500f, 0.000f),
                                            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                            glm::vec2(0.5f, 1.0f)),
                                     Vertex(glm::vec3(-0.433f, -0.250f, 0.000f),
                                            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                            glm::vec2(0.0f, 0.0f)),
                                     Vertex(glm::vec3(0.433f, -0.250f, 0.000f),
                                            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                            glm::vec2(1.0f, 0.0f))}) {}
