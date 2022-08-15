#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

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
        if (isRotating)
        {
            MouseCoordinates currPos = getMouseCoordinates(window);
            double deltaX = currPos.x - previousMousePos.x;
            double deltaY =currPos.y - previousMousePos.y;
            previousMousePos = currPos;

            int width, height;
            glfwGetWindowSize(window, &width, &height);

            float delta_hAngle = MOUSE_SPEED * deltaTime * float(deltaX);
            float delta_vAngle = MOUSE_SPEED * deltaTime * float(deltaY);

            viewMatrix = glm::rotate(viewMatrix,
                delta_hAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix,
                delta_vAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        else
        {
            isRotating = true;
            previousMousePos = getMouseCoordinates(window);
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        isRotating = false;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Camera::translate(int* coords)
{
    modelMatrix = glm::translate(glm::mat4(), glm::vec3(
        static_cast<float>(*coords),
        static_cast<float>(*(coords + 1)),
        static_cast<float>(*(coords + 2))
    ));
}

glm::mat4 Camera::getMVP()
{
    return projectionMatrix * viewMatrix * modelMatrix;
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