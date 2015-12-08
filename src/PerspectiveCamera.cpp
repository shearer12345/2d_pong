#include "PerspectiveCamera.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera (GLSLProgram* glslProgram,
                glm::vec3 position,
                glm::vec3 lookat,
                glm::vec3 upVector)
                : // initializer list follows
                Camera::Camera(glslProgram, position),
                lookat(lookat),
                upVector(upVector)
{
;
}

void PerspectiveCamera::activate()
{
    std::cout << "PerspectiveCamera activated" << std::endl;
    //set projectionMatrix - how we go from 3D to 2D
    glm::mat4 projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(glslProgram->projectionMatrixLocation, 1, false, glm::value_ptr(projectionMatrix));

    //set viewMatrix - how we control the view (viewpoint, view direction, etc)
    glm::mat4 viewMatrix = glm::lookAt(position, lookat, upVector);
    glUniformMatrix4fv(glslProgram->viewMatrixLocation, 1, false, glm::value_ptr(viewMatrix));
}
