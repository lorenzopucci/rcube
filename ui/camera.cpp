#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "camera.hpp"

Camera::Camera(GLFWwindow* window)
{
    lastTime = static_cast<float>(glfwGetTime());

    int width, height;
    glfwGetWindowSize(window, &width, & height);
    projectionMatrix = glm::perspective(glm::radians(FOV),
        float(width)/float(height), 0.1f, 100.0f);

    viewMatrix = glm::lookAt(
        position, 
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
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
            double delta_x = currPos.x - previousMousePos.x;
            double delta_y =currPos.y - previousMousePos.y;
            previousMousePos = currPos;

            int width, height;
            glfwGetWindowSize(window, &width, & height);

            hAngle += MOUSE_SPEED * deltaTime * float(delta_x);
            vAngle += MOUSE_SPEED * deltaTime * float(delta_y);

            //std::cout << "hAngle: " << hAngle << " vAngle: " << vAngle << std::endl;

            position = glm::vec3(
                5.0f * cos(hAngle),
                5.0f * sin(vAngle),
                5.0f * sin(hAngle)
            );

            projectionMatrix = glm::perspective(glm::radians(FOV),
                float(width)/float(height), 0.1f, 100.0f);

            viewMatrix = glm::lookAt(
                position, 
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
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

glm::mat4 Camera::getMVP(GLFWwindow* window)
{
    computeFromInput(window);
    return projectionMatrix * viewMatrix * glm::mat4(1.0);
}

Camera::~Camera()
{
    return;
}