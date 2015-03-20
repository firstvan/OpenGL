#include "myGlew.h"
#include "GLFWProgram.h"
#include "GLProgram.h"
#include "Sphere.h"
#include "GLShader.h"
#include "Plane.h"
#include "Coords.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <defines.h>
#include <stack>
#include <time.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

GLFWwindow * window;

GLProgram prog;
GLProgram prog2;

GLfloat rotateAngle = 0.0f;

Sphere * sphere;
//Plane * plane;


std::stack<glm::mat4> modelStack;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec4 worldLight = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);

glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 camPos = glm::vec3(0.0f, 5.0f, 5.0f);
glm::vec3 camLook = glm::vec3(0.0f, 0.0f, 0.0f);


GLfloat mert1[20];
GLfloat mert2[20];
GLfloat mert3[20];

void setMatrices()
{

    glm::mat4 mv = view * modelStack.top();
    modelStack.pop();
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    sphere = new Sphere(1.0, 300, 300);
    //plane = new Plane(1.0, 1.0, 300, 300);

    GLShader frags(GLShader::GLShaderType::FRAGMENT);
    frags.readShader("phong.frag");
    frags.compileShader();

    GLShader vert(GLShader::GLShaderType::VERTEX);
    vert.readShader("phong.vert");
    vert.compileShader();

    prog.setShaders({ vert.getId(), frags.getId() });
    prog.link();

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);


    model = glm::mat4(1.0f);
    modelStack.push(model);

    view = glm::lookAt(camPos, camLook, up);
    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(70.0f), (float)1280 / 720, 0.1f, 100.0f);
    prog.use();
    prog.setUniform("Light[0].Position", view * worldLight);
    prog.setUniform("Light[0].Intensity", 0.8f, 0.5f, 0.5f);
    worldLight = glm::vec4(7.0f, 5.0f, 5.0f, 1.0f);
    prog.setUniform("Light[1].Position", view * worldLight);
    prog.setUniform("Light[1].Intensity", -0.8f, 0.5f, 0.5f);
    worldLight = glm::vec4(0.0f, 2.0f, 5.0f, 1.0f);
    prog.setUniform("Light[2].Position", view * worldLight);
    prog.setUniform("Light[2].Intensity", 0.0f, 0.1f, 0.5f);
    worldLight = glm::vec4(0.0f, 30.0f, 0.0f, 1.0f);
    prog.setUniform("Light[3].Position", view * worldLight);
    prog.setUniform("Light[3].Intensity", 0.0f, 0.0f, 0.0f);
    worldLight = glm::vec4(10.0f, 0.0f, 5.0f, 1.0f);
    prog.setUniform("Light[4].Position", view * worldLight);
    prog.setUniform("Light[4].Intensity", 0.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < 10; i++)
    {
        mert1[i] = (rand() % 10 - 5);
        mert2[i] = (rand() % 10 - 5);
        mert3[i] = (rand() % 10 - 5);
    }



}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.use();

        //prog.setUniform("Light.Position", view * worldLight);

        prog.setUniform("Material.Kd", 0.2f, 0.8f, 0.3f);
        prog.setUniform("Material.Ka", 0.2f, 0.6f, 0.3f);
        prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.8f);
        prog.setUniform("Material.Shininess", 100.0f);

        model = modelStack.top();
        glm::mat4 eredeti = model;
        //do stuff

        modelStack.push(model);
        setMatrices();

        sphere->render();
        // plane->render();
        for (size_t i = 0; i < 10; i++)
        {

            model = eredeti;

            model *= glm::translate(glm::vec3(mert1[i], mert2[i], mert3[i]));
            modelStack.push(model);
            setMatrices();
            sphere->render();
        }



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void keyFunction(GLFWwindow *window, int key, int scanCode, int action, int mods)
{

    glm::vec3 viewRotVec = glm::vec3(0.0f);
    float viewRotAngle = 0.0f;

    switch (key)
    {
    case GLFW_KEY_UP:
        view = glm::rotate(view, -0.1f, glm::vec3(1.0,0.0,0.0));
        break;
    case GLFW_KEY_DOWN:
        view = glm::rotate(view, 0.1f, glm::vec3(1.0,0.0,0.0));
        break;
    case GLFW_KEY_LEFT:
        view = glm::rotate(view, 0.1f, glm::vec3(0.0, 0.0, 1.0));
        break;
    case GLFW_KEY_RIGHT:
        view = glm::rotate(view, -0.1f, glm::vec3(0.0, 0.0, 1.0));
        break;
    case GLFW_KEY_S:
        view = glm::translate(view, glm::vec3(0.0, 0.1, 0.0));
        break;
    case GLFW_KEY_A:
        view = glm::translate(view, glm::vec3(0.1, 0.0, 0.0));

        break;
    case GLFW_KEY_D:
        view = glm::translate(view, glm::vec3(-0.1, 0.0, 0.1));
        //move camera
        break;
    case GLFW_KEY_W:
        view = glm::translate(view, glm::vec3(0.0, -0.1, 0.0));
        //move camera
        break;
    default:
        printf("Bad key :(\n");
        break;
    }



    glfwPollEvents();
}


int main()
{
    srand(time(NULL));

    window = initWindow("Fifth Lesson", 1280, 720);

    glfwSetKeyCallback(window, keyFunction);

    init();

    mainLoop();

    exit(EXIT_SUCCESS);

}