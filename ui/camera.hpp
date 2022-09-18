#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#define MOUSE_SPEED 0.5f
#define SPEED 3.0f
#define INITIAL_FOV 75.0f

struct MouseCoordinates
{
    double x;
    double y;
};

struct Rotation
{
    float angle;
    glm::vec3 axis;
};

struct Arcball
{
    MouseCoordinates currMouseCoords = {0, 0};
    MouseCoordinates prevMouseCoords = {0, 0};

    float screenWidth = 640;
    float screenHeight = 480;

    bool on;

    Rotation update(const MouseCoordinates &current);
    glm::vec3 getVector(const double &x, const double &y);
};

class Camera
{
public:
    Camera(GLFWwindow* window);
    ~Camera() = default;

    void arrangeBlock(int* coords);
    void scale(const float &factor);
    void resetTransformations();

    void computeFromInput(GLFWwindow* window);
    glm::mat4 getMVP();

    void scrollCallback(GLFWwindow *window, double deltaY);

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;

    float FoV = INITIAL_FOV;
    MouseCoordinates getMouseCoordinates(GLFWwindow* window);

    double lastTime;
    Arcball arcball;
};