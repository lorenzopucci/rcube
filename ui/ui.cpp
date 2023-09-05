/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rcube.hpp>
#include <utility.hpp>

#include "cube.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "ui.hpp"
#include "util.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


void parseConfigFile(Config *cfg)
{
    std::ifstream file("rcube.cfg");

    if (!file.good()) return;

    std::string line;
    while (std::getline(file, line))
    {
        // Remove all spaces
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

        if (line.length() < 3) continue;

        int eqPos = 0;
        for (; eqPos < line.length(); eqPos++)
        {
            if (line[eqPos] == '=') break;
        }
        if (eqPos >= line.length() - 1) continue;

        std::string key = line.substr(0, eqPos);
        std::string sVal = line.substr(eqPos + 1, line.length() - eqPos);
        int val;

        try
        {
            val = std::stoi(sVal);
        }
        catch (const std::invalid_argument& e)
        {
            continue;
        }
        
        if      (key == "scramble_length") cfg->scramble_length = val;
        else if (key == "kociemba_threads") cfg->kociemba_threads = val;
        else if (key == "kociemba_timeout") cfg->kociemba_timeout = val;
    }
}

int rcubeUI::runUI (rcube::Cube *cube, std::function<void(rcube::Move)>
    moveCallback)
{
    glewExperimental = true;
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
        "Rcube GUI", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, "
            "they are not 3.3 compatible.\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return 1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    Camera *camera = new Camera(window);
    Text text = Text(WINDOW_WIDTH, WINDOW_HEIGHT);
    Timer timer(&text, 10, 10);
    Cube cube3d(cube);

    Config config;
    parseConfigFile(&config);

    GlfwUserPtrData *userPtr = new GlfwUserPtrData {camera, &text,
        &timer, &cube3d, moveCallback, config};

    glfwSetWindowUserPointer(window, userPtr);
    glfwSetScrollCallback(window, EventHandler::onScroll);
    glfwSetMouseButtonCallback(window, EventHandler::onClick);
    glfwSetKeyCallback(window, EventHandler::onKey);
    glfwSetCursorPosCallback(window, EventHandler::onDrag);
    glfwSetWindowSizeCallback(window, EventHandler::onResize);


    std::cout << "RCUBE testing interface\n"
        "Copyright (c) 2022 Lorenzo Pucci\n"
        "License: MIT <https://mit-license.org>\n\n"
        "Use your keyboard to apply moves by pressing keys such as R, U, F etc."
        " Holding\nshift will reverse the direction of the moves.\n"
        "Other keyboard shortcuts:\n"
        "- [ctrl+R]: center the view\n"
        "- [ctrl+S]: scramble the cube\n"
        "- [ctrl+D]: solve the cube\n"
        "- [ctrl+T]: start/stop the timer\n\n";
    
    Shader shader(STD_VS, STD_FS);

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!userPtr->MVPupdated)
        {
            shader.setUniformMat4f("MVP", camera->getMVP());
            userPtr->MVPupdated = true;
        }
        if (!userPtr->cubeUpdated)
        {
            cube3d.update();
            userPtr->cubeUpdated = true;
        }
        if (userPtr->hasBeenScrambled && cube->isSolved())
        {
            text.addString("Solved!", -10, 10, 5);
            userPtr->hasBeenScrambled = false;
            timer.stop();
        }

        cube3d.updateAnimations();
        cube3d.draw(camera, &shader);
        timer.refresh();
        text.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (moveCallback != NULL)
    {
    	moveCallback(rcube::Move('U', 0));
    }

    return 0;
}
