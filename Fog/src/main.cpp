#include "myGlew.h"
#include "GLFWProgram.h"
#include "Cube.h"
#include "GLProgram.h"
#include "GLShader.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

GLFWwindow * window;
int WIN_WIDTH = 1600;
int WIN_HEIGHT = 900;
GLProgram program;

Cube * cube;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 eye = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
glm::vec4 worldLight = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);

double orig_x = WIN_WIDTH / 2, orig_y = WIN_HEIGHT/2;

void init()
{
    glfwWindowHint(GLFW_SAMPLES, 15);
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    cube = new Cube(false);

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

    program.use();
    /*program.setUniform("Light[0].Position", view * worldLight);
    program.setUniform("Light[0].Intensity", 0.8f, 0.5f, 0.5f);
    worldLight = glm::vec4(7.0f, 5.0f, 5.0f, 1.0f);
    program.setUniform("Light[1].Position", view * worldLight);
    program.setUniform("Light[1].Intensity", -0.8f, 0.5f, 0.5f);
    */

    program.setUniform("sli.Position", eye);
    program.setUniform("sli.intensity", 0.8f, 0.5f, 0.5f);
    program.setUniform("sli.direction", target);
    program.setUniform("sli.exponent", 3);
    program.setUniform("sli.cutoff", 0.1f);

}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        program.setUniform("Material.Kd", 0.2f, 0.8f, 0.3f);
        program.setUniform("Material.Ka", 0.2f, 0.6f, 0.3f);
        program.setUniform("Material.Ks", 0.2f, 0.2f, 0.8f);
        program.setUniform("Material.Shininess", 100.0f);

        glm::mat4 mv = view * model;
        program.setUniform("ModelViewMatrix", mv);
        program.setUniform("NormalMatrix",
                           glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

        glm::mat4 mvp = projection * mv;
        program.setUniform("MVP", mvp);

        cube->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void mouse_callback(GLFWwindow* window, double x, double y)
{

    target.x -= (x - orig_x) * 0.05;
    target.y += (y - orig_y) * 0.05;
    orig_x = x;
    orig_y = y;
    view = glm::lookAt(eye, target, up);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        eye.y -= 0.1;
        target.y -= 0.1;
        break;
    case GLFW_KEY_S:
        eye.y += 0.1;
        target.y += 0.1;
        break;
    case GLFW_KEY_D:
        eye.x -= 0.1;
        target.x -= 0.1;
        break;
    case GLFW_KEY_A:
        eye.x += 0.1;
        target.x += 0.1;
        break;
    default:
        break;
    }

    view = glm::lookAt(eye, target, up);
}

int main()
{

    window = initWindow("Fog", WIN_WIDTH, WIN_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    init();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    mainLoop();

    exit(EXIT_SUCCESS);
}