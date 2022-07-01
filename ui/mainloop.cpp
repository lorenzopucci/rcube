#include <map>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mainloop.hpp"
#include "cube.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "camera.hpp"

Camera *camera;

// this is terrible, but necessary since glfw does not accept member functions
// as callbacks (std::bind does not work either)
void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
{
    camera->scrollCallback(window, deltaY);
}

std::map<char, Color> colors_new
{
    {'w', {1.0f, 1.0f, 1.0f}},
    {'y', {1.0f, 1.0f, 0.0f}},
    {'g', {0.0f, 1.0f, 0.0f}},
    {'b', {0.0f, 0.0f, 1.0f}},
    {'r', {1.0f, 0.0f, 0.0f}},
    {'o', {1.0f, 0.5f, 0.0f}}
};


void mainLoop (GLFWwindow* window)
{
    VertexArray va;

    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    va.addBuffer(vb, layout);
    IndexBuffer ib(indices, 36);

    camera = new Camera(window);
    glfwSetScrollCallback(window, scrollCallback);

    Shader shader(VERTEX_SHADER, FRAGMENT_SHADER);
    shader.bind();
    char i = 'g';

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniformMat4f("MVP", camera->getMVP(window));
        shader.setUniform4f("u_color",
            colors_new[i].red,
            colors_new[i].green,
            colors_new[i].blue,
            colors_new[i].alpha);
        
        render(&va, &ib, &shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

