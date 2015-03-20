#include "myGlew.h"
#include "GLFWProgram.h"
#include <GLFW\glfw3.h>
#include "Torus.h"
#include <glm\gtx\transform.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

#include "GLProgram.h"
#include "GLShader.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;
GLProgram program;
GLProgram prog;
Torus * torus;

glm::mat4 view;
glm::mat4 model;
glm::mat4 projection;
glm::mat4 model2;




void init()
{
    glfwWindowHint(GLFW_SAMPLES, 15);
    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(0.3f, 0.7f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    torus = new Torus(0.4f, 0.2f, 360, 360);


    GLShader vert1(GLShader::GLShaderType::VERTEX);
    vert1.readShader("diffDonut.vert");
    vert1.compileShader();

    GLShader frag1(GLShader::GLShaderType::FRAGMENT);
    frag1.readShader("diffDonut.frag");
    frag1.compileShader();

    program.setShaders({vert1.getId(), frag1.getId()});
    program.link();

    program.use();
    glm::vec3 kd = glm::vec3(1.0f, 0.0f, 0.0f);
    program.setUniform("kd", kd);

    glm::vec4 light = glm::vec4(5.0f, 5.0f, 5.0f, -1.0f);

    model = glm::mat4(1.0f);

    model *= glm::rotate(45.0f, glm::vec3(1.0, 0.0, 0.0));

    view = glm::mat4(1.0f);

    //glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec4 viewLight = view * light;

    projection = glm::perspective(45.0f, WIDTH / static_cast<float>(HEIGHT),
                                  0.1f, 100.0f);

    model2 = glm::mat4(1.0f);
    model2 *= glm::translate(glm::vec3(0.4, 0.0, 0.1));
    model2 *= glm::rotate(75.0f, glm::vec3(1.0, 0.0, 0.0));

    GLShader vert2(GLShader::GLShaderType::VERTEX);
    vert2.readShader("phongDonut.vert");
    vert2.compileShader();

    GLShader frag2(GLShader::GLShaderType::FRAGMENT);
    frag2.readShader("phongDonut.frag");
    frag2.compileShader();

    prog.setShaders({ vert2.getId(), frag2.getId() });
    prog.link();

    prog.use();

    prog.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.Position", viewLight);
    prog.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);
}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();

        glm::mat4 vm = view * model;
        program.setUniform("ModelViewMatrix", vm);

        glm::mat3 nvm = glm::mat3(glm::vec3(vm[0]), glm::vec3(vm[1]), glm::vec3(vm[2]));
        program.setUniform("NormalMatrix", nvm);

        glm::mat4 mvp = projection * vm;
        program.setUniform("MVP", mvp);

        torus->render();

        prog.use();
        vm = view * model;
        prog.setUniform("ModelViewMatrix", vm);

        nvm = glm::mat3(glm::vec3(vm[0]), glm::vec3(vm[1]), glm::vec3(vm[2]));
        prog.setUniform("NormalMatrix", nvm);

        mvp = projection * view * model2;
        prog.setUniform("MVP", mvp);

        torus->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main()
{
    window = initWindow("Donuts", WIDTH, HEIGHT);

    init();

    mainLoop();

    return 0;
}