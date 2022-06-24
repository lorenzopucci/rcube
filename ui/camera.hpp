#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#define MOUSE_SPEED 0.5f
#define SPEED 3.0f
#define FOV 45.0f

#define INITIAL_VANGLE 0.0f
#define INITIAL_HANGLE 3.14f

struct MouseCoordinates
{
    double x;
    double y;
};

class Camera
{
public:
    Camera(GLFWwindow* window);
    ~Camera();
    void computeFromInput(GLFWwindow* window);
    glm::mat4 getMVP(GLFWwindow* window);

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    float vAngle = INITIAL_VANGLE;
    float hAngle = INITIAL_HANGLE;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    MouseCoordinates getMouseCoordinates(GLFWwindow* window);

    double lastTime;
    bool isRotating = false;
    MouseCoordinates previousMousePos;
};