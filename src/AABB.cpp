#include "AABB.hpp"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max)
{};

AABB::AABB(glm::vec4 min, glm::vec4 max) : min(glm::vec3(min)), max(glm::vec3(max))
{}

AABB::AABB(Mesh * mesh)
{
    min = mesh->vertexData[0].position;
    max = min;

    for(std::vector<Vertex>::iterator it = mesh->vertexData.begin(); it != mesh->vertexData.end(); ++it)
    {
        if (it->position.x < min.x) min.x = it->position.x;
        if (it->position.y < min.y) min.y = it->position.y;
        if (it->position.z < min.z) min.z = it->position.z;

        if (it->position.x > max.x) max.x = it->position.x;
        if (it->position.y > max.y) max.y = it->position.y;
        if (it->position.z > max.z) max.z = it->position.z;

    }
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Mesh bounds are: X(%f->%f) Y(%f->%f) Z(%f->%f) ", min.x, max.x, min.y, max.y, min.z, max.z);
}
