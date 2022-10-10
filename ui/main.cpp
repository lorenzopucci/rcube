/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>
#include <iostream>

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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main ()
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
        "Rubiks cube", NULL, NULL);
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
    rcube::Cube cube = rcube::Cube();
    Text text = Text(WINDOW_WIDTH, WINDOW_HEIGHT);

    GlfwUserPtrData *userPtr = new GlfwUserPtrData {camera, &cube, &text};

    Cube cube3d(cube.blockRender());

    glfwSetWindowUserPointer(window, userPtr);
    glfwSetScrollCallback(window, EventHandler::onScroll);
    glfwSetMouseButtonCallback(window, EventHandler::onClick);
    glfwSetKeyCallback(window, EventHandler::onKey);
    glfwSetCursorPosCallback(window, EventHandler::onDrag);
    glfwSetWindowSizeCallback(window, EventHandler::onResize);

    Shader shader(STD_VS, STD_FS);
    shader.bind();


    std::cout << "RCUBE testing interface\n"
        "Copyright (c) 2022 Lorenzo Pucci\n"
        "License: MIT <https://mit-license.org>\n\n"
        "Use your keyboard to apply moves by pressing keys such as R, U, F etc."
        " Holding\nshift will reverse the direction of the moves. "
        "Press ctrl+R to center the view\nand ctrl+S to scramble the cube.\n\n";
    
    VertexArray va;
    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    va.addBuffer(vb, layout);

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
            cube3d.update(cube.blockRender());
            userPtr->cubeUpdated = true;
        }

        cube3d.draw(&va, &shader, camera);
        text.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
