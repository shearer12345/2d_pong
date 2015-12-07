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
    mesh->glslProgram->use();

    glBindVertexArray(mesh->vertexArrayObject);

    //set projectionMatrix - how we go from 3D to 2D
    glUniformMatrix4fv(mesh->glslProgram->projectionMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0)));

    //set viewMatrix - how we control the view (viewpoint, view direction, etc)
    glUniformMatrix4fv(mesh->glslProgram->viewMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));


    //set modelMatrix and draw for triangle 1
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "model->position: %f, %f, %f", position.x, position.y, position.z);
    glUniformMatrix4fv(mesh->glslProgram->modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // //set modelMatrix and draw for triangle 2
    // modelMatrix = glm::translate(glm::mat4(1.0f), position2);
    // glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glUseProgram(0); //clean up
}
