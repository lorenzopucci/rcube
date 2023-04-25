/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <math.h>
#include <algorithm>
#include <iostream>

#include <rcube.hpp>

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
        float(*coords),
        float(*(coords + 1)),
        float(*(coords + 2))
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

void Camera::centerView()
{
    viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::getMVP()
{
    return projectionMatrix * viewMatrix *  modelMatrix;
}

void EventHandler::onScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    GlfwUserPtrData* userPtr = static_cast<GlfwUserPtrData*>(
        glfwGetWindowUserPointer(window));
    Camera *camera = userPtr->camera;

    camera->FoV -= deltaY;
    if (camera->FoV < 2.0f || camera->FoV > 100.0f)
    {
        camera->FoV += deltaY;
        return;
    }

    camera->projectionMatrix = glm::perspective(glm::radians(camera->FoV),
        float(camera->windowSize.w)/float(camera->windowSize.h), 0.1f, 100.0f);

    userPtr->MVPupdated = false;
}

void EventHandler::onClick(GLFWwindow* window, int button, int action,
    int mods)
{
    Camera* camera = static_cast<GlfwUserPtrData*>(
        glfwGetWindowUserPointer(window))->camera;

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
    GlfwUserPtrData* userPtr = static_cast<GlfwUserPtrData*>(
        glfwGetWindowUserPointer(window));
    Camera *camera = userPtr->camera;

    char newMove = '\0';

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
            break;

        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            camera->pressingShift = action == GLFW_PRESS;
            break;

        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            camera->pressingCtrl = action == GLFW_PRESS;
            break;
        
        case GLFW_KEY_R:
        {
            if (camera->pressingCtrl && action == GLFW_PRESS)
            {
                camera->centerView();
                userPtr->MVPupdated = false;
                break;
            }
            else
            {
                newMove = 'R'; break;
            }
        }
        case GLFW_KEY_L:
            newMove = 'L'; break;
        case GLFW_KEY_U:
            newMove = 'U'; break;
        case GLFW_KEY_D:
        {
            if (camera->pressingCtrl && action == GLFW_PRESS)
            {
                rcube::Algorithm algo = userPtr->cube->solveCfop(true);
                userPtr->cubeUpdated = false;

                if (userPtr->moveCallback == NULL) break;

                for (rcube::Move mv : algo.algorithm)
                {
                    userPtr->moveCallback(mv);
                }
                break;
            }
            newMove = 'D'; break;
        }

    case GLFW_KEY_F:
            newMove = 'F'; break;
        case GLFW_KEY_B:
            newMove = 'B'; break;
        
        case GLFW_KEY_M:
            newMove = 'M'; break;
        case GLFW_KEY_S:
        {
            if (camera->pressingCtrl && action == GLFW_PRESS)
            {
                rcube::Algorithm dest;
                userPtr->cube->scramble(15, &dest);

                std::cout << "Scramble: " << dest.to_string() << std::endl;
                
                userPtr->cubeUpdated = false;
                userPtr->hasBeenScrambled = true;

                userPtr->timer->resume();
                userPtr->timer->start();

                if (userPtr->moveCallback == NULL) break;

                for (rcube::Move mv : dest.algorithm)
                {
                    userPtr->moveCallback(mv);
                }
                break;
            }
            else
            {
                newMove = 'S'; break;
            }
        }
        case GLFW_KEY_E:
            newMove = 'E'; break;
        
        case GLFW_KEY_X:
            newMove = 'x'; break;
        case GLFW_KEY_Y:
            newMove = 'y'; break;
        case GLFW_KEY_Z:
            newMove = 'z'; break;

        case GLFW_KEY_T:
        {
            if (!camera->pressingCtrl || action != GLFW_PRESS) break;

            if (userPtr->timer->_started)
            {
                userPtr->timer->stop();
            }
            else
            {
                userPtr->timer->resume();
                userPtr->timer->start();
            }
            break;
        }
    }

    if (newMove == '\0' || action == GLFW_RELEASE) return;


    rcube::Move move(newMove, 1 - (camera->pressingShift * 2));
    userPtr->cube->performMove(move);
    userPtr->cubeUpdated = false;

    if (userPtr->moveCallback != NULL)
    {
        userPtr->moveCallback(move);
    }
}

void EventHandler::onDrag(GLFWwindow* window, double xpos, double ypos)
{
    GlfwUserPtrData* userPtr = static_cast<GlfwUserPtrData*>(
        glfwGetWindowUserPointer(window));
    Camera *camera = userPtr->camera;

    if (!camera->arcball.on) return;

    double currTime = static_cast<float>(glfwGetTime());
    float deltaTime = float(currTime - camera->lastTime);
    camera->lastTime = currTime;
    
    Rotation rot = camera->arcball.update({xpos, ypos});

    if (rot.axis.x != 0.0f || rot.axis.y != 0.0f || rot.axis.z != 0.0f)
    {
        glm::vec3 axis = glm::inverse(glm::mat3(camera->viewMatrix)) * rot.axis;
        camera->viewMatrix = glm::rotate(camera->viewMatrix, rot.angle, axis);

        userPtr->MVPupdated = false;
    }
}

void EventHandler::onResize(GLFWwindow* window, int width, int height)
{
    GlfwUserPtrData* userPtr = static_cast<GlfwUserPtrData*>(
        glfwGetWindowUserPointer(window));
    Camera *camera = userPtr->camera;

    camera->windowSize = {width, height};
    camera->arcball.windowSize = {width, height};
    camera->projectionMatrix = glm::perspective(glm::radians(camera->FoV),
        float(camera->windowSize.w)/float(camera->windowSize.h), 0.1f, 100.0f);
    
    glViewport(0, 0, width, height);

    userPtr->MVPupdated = false;

    userPtr->text->onResize(width, height);
}