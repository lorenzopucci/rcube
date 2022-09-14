#include <map>
#include <functional>

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

/*Camera *camera;

// this is terrible, but necessary since glfw does not accept member functions
// as callbacks (std::bind does not work either)
void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
{
    camera->scrollCallback(window, deltaY);
}
*/

void mainloop (GLFWwindow* window, rcube::Cube* cube)
{
    VertexArray va;

    Camera *camera = new Camera(window);
    //glfwSetScrollCallback(window, scrollCallback);

    Shader shader(VERTEX_SHADER, FRAGMENT_SHADER);

    //cube->performMove(rcube::Move('R', 1));
    Cube cube3d(cube->blockRender());

    VertexBuffer vb(vertices, 8 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    va.addBuffer(vb, layout);

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        camera->computeFromInput(window);
        shader.setUniformMat4f("MVP", camera->getMVP());

        cube3d.draw(&va, &shader, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

