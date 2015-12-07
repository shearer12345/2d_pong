#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Mesh.hpp"

class Model {
public:
    glm::vec3 position;
    glm::vec3 velocity;

    Mesh * mesh;

    Model(Mesh* mesh,
          glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f)
      );

    void draw();
};
