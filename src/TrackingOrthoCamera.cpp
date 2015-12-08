#include "TrackingOrthoCamera.hpp"

TrackingOrthoCamera::TrackingOrthoCamera(GLSLProgram* glslProgram, glm::vec3* target, glm::vec2 size)
                :
                OrthoCamera::OrthoCamera(glslProgram, *target, size),
                target(target)
{

}

void TrackingOrthoCamera::activate()
{
    position = *target;
    SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "TrackingOrthoCamera activated. Target is %f,%f", target->x, target->y);

    //set projectionMatrix - how we go from 3D to 2D
    glm::vec2 halfSize = size / 2.0f;
    glm::mat4 projectionMatrix = glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);
    glUniformMatrix4fv(glslProgram->projectionMatrixLocation, 1, false, glm::value_ptr(projectionMatrix));

    //set viewMatrix - how we control the view (viewpoint, view direction, etc)
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(position.x, position.y, 1.0f), glm::vec3(position.x, position.y, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glslProgram->viewMatrixLocation, 1, false, glm::value_ptr(viewMatrix));
};
