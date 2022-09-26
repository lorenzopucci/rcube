#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
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
        2 * x / windowSize.w - 1.0f,
        -2 * y / windowSize.h + 1.0f,
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

    projectionMatrix = glm::perspective(glm::radians(FoV),
        float(windowSize.w)/float(windowSize.h), 0.1f, 100.0f);

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

void EventHandler::onScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    camera->FoV -= deltaY;
    if (camera->FoV < 2.0f || camera->FoV > 100.0f)
    {
        camera->FoV += deltaY;
        return;
    }

    camera->projectionMatrix = glm::perspective(glm::radians(camera->FoV),
        float(camera->windowSize.w)/float(camera->windowSize.h), 0.1f, 100.0f);

    camera->updated = false;
}

void EventHandler::onClick(GLFWwindow* window, int button, int action,
    int mods)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (button != GLFW_MOUSE_BUTTON_LEFT) return;
    
    if (action == GLFW_PRESS)
    {
        camera->arcball.on = true;
        camera->arcball.currMouseCoords = camera->getMouseCoordinates(window);
    }
    else if (action == GLFW_RELEASE)
    {
        camera->arcball.on = false;
    }
}

void EventHandler::onKey(GLFWwindow* window, int key, int scancode, int action,
    int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void EventHandler::onDrag(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (!camera->arcball.on) return;

    double currTime = static_cast<float>(glfwGetTime());
    float deltaTime = float(currTime - camera->lastTime);
    camera->lastTime = currTime;
    
    Rotation rot = camera->arcball.update({xpos, ypos});

    if (rot.axis.x != 0.0f || rot.axis.y != 0.0f || rot.axis.z != 0.0f)
    {
        glm::vec3 axis = glm::inverse(glm::mat3(camera->viewMatrix)) * rot.axis;
        camera->viewMatrix = glm::rotate(camera->viewMatrix, rot.angle, axis);

        camera->updated = false;
    }
}

void EventHandler::onResize(GLFWwindow* window, int width, int height)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    camera->windowSize = {width, height};
    camera->arcball.windowSize = {width, height};
    camera->projectionMatrix = glm::perspective(glm::radians(camera->FoV),
        float(camera->windowSize.w)/float(camera->windowSize.h), 0.1f, 100.0f);
    
    glViewport(0, 0, width, height);

    camera->updated = false;
}