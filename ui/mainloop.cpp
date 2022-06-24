#include <map>

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

struct color
{
    float red;
    float green;
    float blue;
    float alpha = 1.0f;
};

std::map<char, color> colorMapping
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

    Renderer renderer;
    Camera camera(window);

    Shader shader(VERTEX_SHADER, FRAGMENT_SHADER);
    shader.bind();
    
    char i = 'g';

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniformMat4f("MVP", camera.getMVP(window));
        shader.setUniform4f("u_color",
            colorMapping[i].red,
            colorMapping[i].green,
            colorMapping[i].blue,
            colorMapping[i].alpha);
        
        renderer.draw(&va, &ib, &shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

