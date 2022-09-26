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

struct WindowSize
{
    int w;
    int h;
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

    WindowSize windowSize = {640, 480};

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
    glm::mat4 getMVP();

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    bool updated = false; // false when MPV needs to be sent to the GPU

    float FoV = INITIAL_FOV;
    MouseCoordinates getMouseCoordinates(GLFWwindow* window);
    WindowSize windowSize = {640, 480};

    double lastTime;
    Arcball arcball;
};

namespace EventHandler
{
    void onScroll(GLFWwindow* window, double deltaX, double deltaY);
    void onClick(GLFWwindow* window, int button, int action, int mods);
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onDrag(GLFWwindow* window, double xpos, double ypos);
    void onResize(GLFWwindow* window, int width, int height);
};