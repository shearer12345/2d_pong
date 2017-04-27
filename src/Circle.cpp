#include "Circle.hpp"

#include <glm/gtc/constants.hpp>

Circle::Circle(GLSLProgram *glslProgram, int segments)
    : Mesh::Mesh(glslProgram), segments(segments) {
  const static float TWO_PI = glm::two_pi<float>();
  GLfloat anglePerSegmentRads = TWO_PI / (float)segments;

  // top
  vertexData.push_back(Vertex(glm::vec3(0.0f, 1.0f, 0.0f),
                              glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                              glm::vec2(0.5f, 1.0f)));

  for (int i = 1; i <= segments; ++i) {
    // center
    vertexData.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                glm::vec2(0.5f, 0.5f)));

    GLfloat angleRads = anglePerSegmentRads * i;
    GLfloat x = sin(angleRads);
    GLfloat y = cos(angleRads);
    Vertex v =
        Vertex(glm::vec3(x, y, 0.000f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
               glm::vec2(0.5 * (x + 1), 0.5 * (y + 1)));

    vertexData.push_back(v);
    if (i < segments)
      vertexData.push_back(v);
  }
  bufferData();
}
