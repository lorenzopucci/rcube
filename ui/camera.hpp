#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#define MOUSE_SPEED 0.5f
#define SPEED 3.0f
#define INITIAL_FOV 45.0f

struct MouseCoordinates
{
    double x;
    double y;
};

class Camera
{
public:
    Camera(GLFWwindow* window);
    ~Camera() = default;

    void computeFromInput(GLFWwindow* window);
    void scrollCallback(GLFWwindow *window, double deltaY);
    void arrangeBlock(int* coords);
    void scale(const float &factor);
    void resetTransformations();

    glm::mat4 getMVP();

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    float FoV = INITIAL_FOV;
    MouseCoordinates getMouseCoordinates(GLFWwindow* window);

    double lastTime;
    bool isRotating = false;
    MouseCoordinates previousMousePos;
};