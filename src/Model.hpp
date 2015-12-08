#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>

#include "Mesh.hpp"
#include "AABB.hpp"

class Model {
public:
    const std::string name;
    glm::vec3 position;
    void setVelocity(glm::vec3 velocity);
    glm::vec3 getVelocity();

    glm::vec3 scale = glm::vec3(1.0f);
    float angleDegs = 0.0f;
    float angleVelocityDegs = 0.0f;

    bool moveable = true;
    uint bounceAxis = 0;

    Mesh * mesh;
    AABB * aabb;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 modelMatrixNoRotation = glm::mat4(1.0f);

    Model(  std::string name,
            Mesh* mesh,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f),
            bool moveable = true
      );

    AABB* getAABB();
    void draw();
    void updateModelMatrix();
    static bool intersects(Model* first, Model* second);

protected:
    glm::vec3 _velocity;
};
