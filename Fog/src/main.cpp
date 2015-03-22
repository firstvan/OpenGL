#include "myGlew.h"
#include "GLFWProgram.h"
#include "Cube.h"
#include "GLProgram.h"
#include "GLShader.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>


GLFWwindow * window;
int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;
GLProgram program;

Cube * cube;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 eye = glm::vec3(1.0, 2.0, 1.0);
glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);


void init()
{
    glfwWindowHint(GLFW_SAMPLES, 15);
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    cube = new Cube(true);

    GLShader vert(GLShader::GLShaderType::VERTEX);
    vert.readShader("fog.vert");
    vert.compileShader();
    GLShader frag(GLShader::GLShaderType::FRAGMENT);
    frag.readShader("fog.frag");
    frag.compileShader();

    program.setShaders({ vert.getId(), frag.getId() });
    program.link();

    model = glm::mat4(1.0f);

    view = glm::lookAt(eye, target, up);

    projection = glm::perspective(45.0f, (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 100.0f);

}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        program.use();

        glm::mat4 mvp = projection * view * model;
        program.setUniform("MVP", mvp);
        cube->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

int main()
{

    window = initWindow("Fog", WIN_WIDTH, WIN_HEIGHT);

    init();

    mainLoop();

    exit(EXIT_SUCCESS);
}