#include "Model.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(std::string name,
             Mesh * mesh,
             glm::vec3 position,
             glm::vec3 velocity,
             bool moveable)
             : // initializer list follows
             name(name),
             mesh(mesh),
             aabb(new AABB(mesh)),
             position(position),
             _velocity(velocity),
             moveable(moveable)
{
    if (!moveable) _velocity = glm::vec3(0.0f);
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Model created. Named %s ", name.c_str());
}

void Model::setVelocity(glm::vec3 velocity)
{
    if (moveable) _velocity = velocity;
}

glm::vec3 Model::getVelocity()
{
     return _velocity;
}


void Model::draw()
{
    updateModelMatrix();
    glUniformMatrix4fv(mesh->glslProgram->modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));

    mesh->draw();
}

AABB* Model::getAABB()
{
    return aabb;
}

void Model::updateModelMatrix()
{
    glm::mat4 tmp = glm::mat4(1.0f);
    tmp = glm::translate(tmp, position);
    tmp = glm::scale(tmp, scale);

    modelMatrix = tmp;
}

bool Model::intersects(Model* first, Model* second)
{
    first->updateModelMatrix();
    second->updateModelMatrix();
    glm::mat4 firstMM = first->modelMatrix;
    glm::mat4 secondMM = second->modelMatrix;

    AABB firstTransformed = AABB(first->modelMatrix * glm::vec4(first->aabb->min, 1.0f), first->modelMatrix * glm::vec4(first->aabb->max, 1.0f));
    AABB secondTransformed = AABB(second->modelMatrix * glm::vec4(second->aabb->min, 1.0f), second->modelMatrix * glm::vec4(second->aabb->max, 1.0f));

    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "first transformed mesh bounds are: X(%f->%f) Y(%f->%f) Z(%f->%f) ", firstTransformed.min.x, firstTransformed.max.x, firstTransformed.min.y, firstTransformed.max.y, firstTransformed.min.z, firstTransformed.max.z);
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "second transformed mesh bounds are: X(%f->%f) Y(%f->%f) Z(%f->%f) ", secondTransformed.min.x, secondTransformed.max.x, secondTransformed.min.y, secondTransformed.max.y, secondTransformed.min.z, secondTransformed.max.z);

    glm::bvec3 overlapXYZ = glm::bvec3(false);
    for(int i = 0; i < 3; ++i)
    {
        overlapXYZ[i] = firstTransformed.min[i] <= secondTransformed.max[i] && firstTransformed.max[i] >= secondTransformed.min[i];
    }

    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "Overlaps are X(%d) Y(%d) Z(%d) ", overlapXYZ.x, overlapXYZ.y, overlapXYZ.z);

    return overlapXYZ.x && overlapXYZ.y && overlapXYZ.z;
}
