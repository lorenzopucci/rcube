#include <map>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rcube.hpp>
#include <utility.hpp>

#include "mainloop.hpp"
#include "cube.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "camera.hpp"


void mainloop (GLFWwindow* window, rcube::Cube* cube)
{
    VertexArray va;

    Camera *camera = new Camera(window);

    glfwSetWindowUserPointer(window, camera);
    glfwSetScrollCallback(window, EventHandler::onScroll);
    glfwSetMouseButtonCallback(window, EventHandler::onClick);
    glfwSetKeyCallback(window, EventHandler::onKey);
    glfwSetCursorPosCallback(window, EventHandler::onDrag);
    glfwSetWindowSizeCallback(window, EventHandler::onResize);

    Shader shader(VERTEX_SHADER, FRAGMENT_SHADER);
    shader.bind();

    rcube::Algorithm dest;
    cube->scramble(12, &dest);
    std::cout << dest.to_string() << std::endl;
    Cube cube3d(cube->blockRender());

    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    va.addBuffer(vb, layout);

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!camera->updated)
        {
            shader.setUniformMat4f("MVP", camera->getMVP());
            camera->updated = true;
        }

        cube3d.draw(&va, &shader, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

