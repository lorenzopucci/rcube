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
    glfwSetWindowUserPointer(window, camera);
    glfwSetScrollCallback(window, EventHandler::onScroll);
    glfwSetMouseButtonCallback(window, EventHandler::onClick);
    glfwSetKeyCallback(window, EventHandler::onKey);
    glfwSetCursorPosCallback(window, EventHandler::onDrag);
    glfwSetWindowSizeCallback(window, EventHandler::onResize);

    Shader shader(VERTEX_SHADER, FRAGMENT_SHADER);
    shader.bind();

    rcube::Cube cube = rcube::Cube();
    rcube::Algorithm dest;
    cube.scramble(12, &dest);
    std::cout << dest.to_string() << std::endl;
    Cube cube3d(cube.blockRender());
    
    VertexArray va;
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
        while (!camera->movesQueue.empty())
        {
            cube.performMove(camera->movesQueue.front());
            camera->movesQueue.pop();
            cube3d.update(cube.blockRender());
        }

        cube3d.draw(&va, &shader, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
