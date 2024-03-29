/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <functional>

#define MOUSE_SPEED 0.5f
#define SPEED 3.0f
#define INITIAL_FOV 75.0f
#define ROT_SPEED 5.0f

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

    void arrangeBlock(const glm::vec3 &pos, const glm::quat &orient);
    void scale(const float &factor);
    void resetTransformations();
    void centerView();
    glm::mat4 getMVP();

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;

    float FoV = INITIAL_FOV;
    MouseCoordinates getMouseCoordinates(GLFWwindow* window);
    WindowSize windowSize = {640, 480};

    double lastTime;
    Arcball arcball;

    bool pressingShift = false;
    bool pressingCtrl = false;
};

namespace EventHandler
{
    void onScroll(GLFWwindow* window, double deltaX, double deltaY);
    void onClick(GLFWwindow* window, int button, int action, int mods);
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onDrag(GLFWwindow* window, double xpos, double ypos);
    void onResize(GLFWwindow* window, int width, int height);
};
