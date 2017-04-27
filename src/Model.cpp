#include "Model.hpp"

#include "SDL.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(std::string name, Mesh *mesh, Texture *texture, glm::vec3 position,
             glm::vec3 velocity,
             bool moveable)
    : // initializer list follows
      name(name),
      mesh(mesh), texture(texture), aabb(new AABB(mesh)), position(position),
      _velocity(velocity), moveable(moveable) {
  if (!moveable)
    _velocity = glm::vec3(0.0f);
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG,
                 "Model created. Named %s ", name.c_str());
}

void Model::setVelocity(glm::vec3 velocity) {
  if (moveable) {
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                   "Model velocity set on %s to %f,%f,%f", name.c_str(),
                   velocity.x, velocity.y, velocity.z);
    _velocity = velocity;
  }
}

glm::vec3 Model::getVelocity() { return _velocity; }

void Model::draw() {
  updateModelMatrix();
  glUniformMatrix4fv(mesh->glslProgram->modelMatrixLocation, 1, false,
                     glm::value_ptr(modelMatrix));

  glUniform1i(mesh->glslProgram->textureSamplerLocation,
              0);               // make texture unit 0 feed our textureSampler
  glActiveTexture(GL_TEXTURE0); // make texture unit 0 the active texture unit
                                // (which texture unit subsequent texture state
                                // calls will	affect)

  glBindTexture(
      GL_TEXTURE_2D,
      texture
          ->textureID); // make our texture object feed the active texture unit

  mesh->draw();
}

AABB *Model::getAABB() { return aabb; }

void Model::updateModelMatrix() {
  glm::mat4 tmp = glm::mat4(1.0f);
  tmp = glm::translate(tmp, position);
  tmp = glm::scale(tmp, scale);
  modelMatrixNoRotation = tmp;

  tmp = glm::rotate(tmp, glm::radians(angleDegs), glm::vec3(0.0f, 0.0f, 1.0f));

  modelMatrix = tmp;
}

bool Model::intersects(Model *first, Model *second) {
  first->updateModelMatrix();
  second->updateModelMatrix();

  AABB firstTransformed =
      AABB(first->modelMatrixNoRotation * glm::vec4(first->aabb->min, 1.0f),
           first->modelMatrixNoRotation * glm::vec4(first->aabb->max, 1.0f));
  AABB secondTransformed =
      AABB(second->modelMatrixNoRotation * glm::vec4(second->aabb->min, 1.0f),
           second->modelMatrixNoRotation * glm::vec4(second->aabb->max, 1.0f));

  SDL_LogMessage(
      SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
      "first transformed mesh bounds are: X(%f->%f) Y(%f->%f) Z(%f->%f) ",
      firstTransformed.min.x, firstTransformed.max.x, firstTransformed.min.y,
      firstTransformed.max.y, firstTransformed.min.z, firstTransformed.max.z);
  SDL_LogMessage(
      SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
      "second transformed mesh bounds are: X(%f->%f) Y(%f->%f) Z(%f->%f) ",
      secondTransformed.min.x, secondTransformed.max.x, secondTransformed.min.y,
      secondTransformed.max.y, secondTransformed.min.z,
      secondTransformed.max.z);

  glm::bvec3 intersectionXYZ = glm::bvec3(false);
  for (int i = 0; i < 3; ++i) {
    intersectionXYZ[i] = firstTransformed.min[i] <= secondTransformed.max[i] &&
                         firstTransformed.max[i] >= secondTransformed.min[i];
  }

  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                 "Axis Overlaps are X(%d) Y(%d) Z(%d) ", intersectionXYZ.x,
                 intersectionXYZ.y, intersectionXYZ.z);

  return intersectionXYZ.x && intersectionXYZ.y && intersectionXYZ.z;
}
