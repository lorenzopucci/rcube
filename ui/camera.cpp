#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>
#include <algorithm>

#include "camera.hpp"


Rotation Arcball::update(const MouseCoordinates &current)
{
    this->prevMouseCoords = this->currMouseCoords;
    this->currMouseCoords = current;

    glm::vec3 currVec = getVector(currMouseCoords.x, currMouseCoords.y);
    glm::vec3 prevVec = getVector(prevMouseCoords.x, prevMouseCoords.y);

    float angle = SPEED * acos(std::min(1.0f, glm::dot(prevVec, currVec)));
    glm::vec3 axis = glm::cross(prevVec, currVec);

    return {angle, axis};
}

glm::vec3 Arcball::getVector(const double &x, const double &y)
{
    glm::vec3 vector = glm::vec3(
        2 * x / screenWidth - 1.0f,
        -2 * y / screenHeight + 1.0f,
        0.0f
    );

    float zSquared = pow(vector.x, 2) + pow(vector.y, 2);

    if (zSquared > 1.0f) return glm::normalize(vector);

    vector.z = sqrt(1.0f - zSquared);
    return vector;
}

Camera::Camera(GLFWwindow* window)
{
    lastTime = static_cast<float>(glfwGetTime());

    int width, height;
    glfwGetWindowSize(window, &width, & height);

    projectionMatrix = glm::perspective(glm::radians(FoV),
        float(width)/float(height), 0.1f, 100.0f);

    viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f), // camera position
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    modelMatrix = glm::mat4(1.0f);
}

MouseCoordinates Camera::getMouseCoordinates(GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return (MouseCoordinates){x, y};
}

void Camera::computeFromInput(GLFWwindow* window)
{
    double currTime = static_cast<float>(glfwGetTime());
    float deltaTime = float(currTime - lastTime);
    lastTime = currTime;
    

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (arcball.on)
        {
            Rotation rot = arcball.update(getMouseCoordinates(window));

            if (rot.axis.x != 0.0f || rot.axis.y != 0.0f || rot.axis.z != 0.0f)
            {
                glm::vec3 axis = glm::inverse(glm::mat3(viewMatrix)) * rot.axis;
                viewMatrix = glm::rotate(viewMatrix, rot.angle, axis);
            }
        }
        else
        {
            arcball.on = true;
            arcball.currMouseCoords = getMouseCoordinates(window);
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        arcball.on = false;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Camera::arrangeBlock(int* coords)
{
    // multiplied by 1.1f to leave a small gap between blocks
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(
        static_cast<float>(*coords) * 1.1f,
        static_cast<float>(*(coords + 1)) * 1.1f,
        static_cast<float>(*(coords + 2)) * 1.1f
    ));
}

void Camera::scale(const float &factor)
{
    modelMatrix = glm::scale(glm::mat4(1.0f),
        glm::vec3(factor, factor, factor));
}

void Camera::resetTransformations()
{
    modelMatrix = glm::mat4(1.0f);
}

glm::mat4 Camera::getMVP()
{
    return projectionMatrix * viewMatrix *  modelMatrix;
}

void Camera::scrollCallback(GLFWwindow *window, double deltaY)
{
    FoV -= deltaY;
    if (FoV < 2.0f || FoV > 80.0f)
    {
        FoV += deltaY;
        return;
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    projectionMatrix = glm::perspective(glm::radians(FoV),
        float(width)/float(height), 0.1f, 100.0f);
}