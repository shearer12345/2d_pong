#pragma once

#include <glm/glm.hpp>
#include <string>

#include "AABB.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

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
  unsigned int bounceAxis = 0;

  Mesh *mesh;
  AABB *aabb;
  Texture *texture;

  glm::mat4 modelMatrix = glm::mat4(1.0f);
  glm::mat4 modelMatrixNoRotation = glm::mat4(1.0f);

  Model(std::string name, Mesh *mesh, Texture *texture,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f), bool moveable = true);

  AABB *getAABB();
  void draw();
  void updateModelMatrix();
  static bool intersects(Model *first, Model *second);

protected:
  glm::vec3 _velocity;
};
