#include "Model.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(Mesh * mesh,
             glm::vec3 position,
             glm::vec3 velocity)
             : // initializer list follows
             mesh(mesh),
             position(position),
             velocity(velocity)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "New model with GLSL programID %i", mesh->glslProgram->programID);
}

void Model::draw()
{
    //set modelMatrix and draw for triangle 1
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(mesh->glslProgram->modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));

    mesh->draw();
}
