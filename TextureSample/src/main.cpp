#include "myGlew.h"
#include "GLFWProgram.h"
#include "Cube.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "TextureReader.h"
#include "Sphere.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/geometric.hpp>
#include <iostream>


using glm::vec3;
using glm::vec4;
using glm::mat4;

int WIN_WIDTH = 1600;
int WIN_HEIGHT = 900;
bool needInit = true;
double last_x = 0;
double last_y =  0;

GLFWwindow * window;
GLProgram prog;

Cube * enviroment;
Sphere * sphere;
Cube * box;

GLfloat ter = 50.0f;
vec3 camPos = vec3(-ter + 1, ter - 1, - ter + 1);
vec3 camTarget;
vec3 camUp(0.0f, 1.0f, 0.0f);

mat4 view;
mat4 model;
mat4 projection;

TextureReader textureReader;

GLuint sphereTextureColorBind;
GLuint sphereTextureTransBind;

GLuint envTextureBind;
GLuint boxTextureBind;

vec3 sphereCenter(0.0f, 0.0f, 0.0f);

bool keyStates[1024];
std::vector<vec3> normals;

mat4 movedBox = mat4(1.0f);
vec3 boxCenter;
float boxSize = 5.0f;

GLuint loadEnviromentTexture()
{
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    return textureReader.loadTex("wall.tga", w, h);
}

void loadSphereTexture()
{
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    sphereTextureColorBind = textureReader.loadTex("sphereColor.tga",w,h);
    glActiveTexture(GL_TEXTURE1);
    sphereTextureTransBind = textureReader.loadTex("sphereTrans.tga", w, h);
}

void setLights()
{
    prog.setUniform("viewPos", camPos);
    prog.setUniform("material.shininess", 32.0f);


    prog.setUniform("light.position", camPos);
    prog.setUniform("light.cutOff", glm::cos(glm::radians(5.0f)));

    prog.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
    prog.setUniform("light.diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("light.specular", 1.0f, 1.0f, 1.0f);



    /*prog.setUniform("spotLight[1].position", enviroment->getVertex(21));
    prog.setUniform("spotLight[1].direction", vec3(0.0f, -1.0f, 0.0f));
    prog.setUniform("spotLight[1].cutOff", glm::cos(glm::radians(10.0f)));
    prog.setUniform("spotLight[1].outerCutOff", glm::cos(glm::radians(20.0f)));
    prog.setUniform("spotLight[1].ambient", vec3(0.2f));
    prog.setUniform("spotLight[1].diffuse", vec3(0.8f));
    prog.setUniform("spotLight[1].specular", vec3(1.0f));

    prog.setUniform("spotLight[2].position", enviroment->getVertex(22));
    prog.setUniform("spotLight[2].direction", vec3(0.0f, -5.0f, 0.0f));
    prog.setUniform("spotLight[2].cutOff", glm::cos(glm::radians(10.0f)));
    prog.setUniform("spotLight[2].outerCutOff", glm::cos(glm::radians(20.0f)));
    prog.setUniform("spotLight[2].ambient", vec3(0.2f));
    prog.setUniform("spotLight[2].diffuse", vec3(0.8f));
    prog.setUniform("spotLight[2].specular", vec3(1.0f));

    prog.setUniform("spotLight[3].position", enviroment->getVertex(23));
    prog.setUniform("spotLight[3].direction", vec3(0.0f, -1.0f, 0.0f));
    prog.setUniform("spotLight[3].cutOff", glm::cos(glm::radians(10.0f)));
    prog.setUniform("spotLight[3].outerCutOff", glm::cos(glm::radians(20.0f)));
    prog.setUniform("spotLight[3].ambient", vec3(0.2f));
    prog.setUniform("spotLight[3].diffuse", vec3(0.8f));
    prog.setUniform("spotLight[3].specular", vec3(1.0f));
    */
    prog.setUniform("pointLight.position", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("pointLight.ambient", vec3(0.2f));
    prog.setUniform("pointLight.diffuse", vec3(0.2f));
    prog.setUniform("pointLight.specular", vec3(0.2f));
}

void init()
{
    for (int i = 0; i < 1024; i++)
    {
        keyStates[i] = false;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLShader vertexShader(GLShader::GLShaderType::VERTEX);
    vertexShader.readShader("vertex.vert");
    vertexShader.compileShader();

    GLShader fragmentShader(GLShader::GLShaderType::FRAGMENT);
    fragmentShader.readShader("fragment.frag");
    fragmentShader.compileShader();

    prog.setShaders({ vertexShader.getId(), fragmentShader.getId() });
    prog.link();

    prog.use();

    enviroment = new Cube(true, ter * 2);

    projection = mat4(1.0f);
    projection = glm::perspective(glm::radians(50.0f), (float)WIN_WIDTH / WIN_HEIGHT, 0.001f, 500.0f);

    envTextureBind = loadEnviromentTexture();

    sphere = new Sphere(5.0f, 180, 180);
    sphere->dir = vec3(1.0f, 1.0f, 1.0f);

    model = mat4(1.0f);
    view = glm::lookAt(camPos, sphereCenter, camUp);
    setLights();

    loadSphereTexture();

    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));

    box = new Cube(false, boxSize * 2);

    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    boxTextureBind = textureReader.loadTex("box.tga", w, h);

    boxCenter = vec3(15.0f, 0.0f, 0.0f);
    movedBox = mat4(1.0f);
    movedBox = glm::translate(boxCenter);
    view = glm::lookAt(camPos, sphereCenter, camUp);
}

void setMatrices()
{

    mat4 mv =view * mat4(1.0f);
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    mat4 mvp = projection * mv;
    prog.setUniform("MVP", mvp);
}


void moveBox();

void envReflect();

void boxReflect();

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        moveBox();
        view = glm::lookAt(camPos, sphereCenter, camUp);

        vec3 lightDirection = boxCenter - camPos;
        prog.setUniform("light.direction", lightDirection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, envTextureBind);
        setMatrices();
        prog.setUniform("needMix", false);
        enviroment->render();

        envReflect();

        boxReflect();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sphereTextureColorBind);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sphereTextureTransBind);


        vec3 d = sphere->dir * 0.5f;
        sphereCenter += d;
        model *= glm::translate(d);
        mat4 mv = view * model;
        prog.setUniform("ModelViewMatrix", mv);
        prog.setUniform("NormalMatrix",
                        glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

        prog.setUniform("MVP", projection * mv);
        prog.setUniform("needMix", true);
        sphere->render();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTextureBind);

        mv = view * movedBox;
        prog.setUniform("ModelViewMatrix", mv);
        prog.setUniform("NormalMatrix",
                        glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
        mat4 mvp = projection * mv;
        prog.setUniform("MVP", mvp);
        prog.setUniform("needMix", false);
        box->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void moveBox()
{
    if (keyStates[GLFW_KEY_I])
    {
        if (boxCenter.z + boxSize < ter)
        {
            boxCenter.z += 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_K])
    {
        if (boxCenter.z - boxSize > -ter)
        {
            boxCenter.z -= 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_J])
    {
        if (boxCenter.x + boxSize < ter)
        {
            boxCenter.x += 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_L])
    {
        if (boxCenter.x - boxSize > -ter)
        {
            boxCenter.x -= 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_U])
    {
        if (boxCenter.y + boxSize < ter)
        {
            boxCenter.y += 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_H])
    {
        if (boxCenter.y - boxSize > -ter)
        {
            boxCenter.y -= 0.1;
            movedBox = glm::translate(boxCenter);
        }
    }
}

void keyFunction(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        keyStates[key] = false;
    }


    glfwPollEvents();
}



int main()
{
    window = initWindow("Beadando", WIN_WIDTH, WIN_HEIGHT, false);

    init();

    glfwSetKeyCallback(window, keyFunction);

    mainLoop();

    return 0;
}

void envReflect()
{
    if (sphereCenter.x > ter - 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[3]);
    }

    if (sphereCenter.x <  -ter + 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[1]);
    }

    if (sphereCenter.z > ter - 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[0]);
    }

    if (sphereCenter.z <  -ter + 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[2]);
    }

    if (sphereCenter.y > ter - 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[5]);
    }

    if (sphereCenter.y <  -ter + 5.0f)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[4]);
    }

}

GLfloat dist(GLfloat x, GLfloat y)
{
    return glm::abs(x - y);
}

void boxReflect()
{
    if (sphereCenter.x - 5.0f <= boxCenter.x + boxSize
            && sphereCenter.x >= boxCenter.x - boxSize
            && sphereCenter.y <= boxCenter.y + boxSize
            && sphereCenter.y >= boxCenter.y - boxSize
            && sphereCenter.z <= boxCenter.z + boxSize
            && sphereCenter.z >= boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(1.0f, 0.0f, 0.0f));
    }

    if (sphereCenter.x + 5.0f >= boxCenter.x - boxSize
            && sphereCenter.x <= boxCenter.x + boxSize
            && sphereCenter.y <= boxCenter.y + boxSize
            && sphereCenter.y >= boxCenter.y - boxSize
            && sphereCenter.z <= boxCenter.z + boxSize
            && sphereCenter.z >= boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(-1.0f, 0.0f, 0.0f));
    }

    if (sphereCenter.y - 5.0f <= boxCenter.y + boxSize
            && sphereCenter.x >= boxCenter.x - boxSize
            && sphereCenter.x <= boxCenter.x + boxSize
            && sphereCenter.y >= boxCenter.y - boxSize
            && sphereCenter.z <= boxCenter.z + boxSize
            && sphereCenter.z >= boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 1.0f, 0.0f));
    }

    if (sphereCenter.y + 5.0f >= boxCenter.y - boxSize
            && sphereCenter.x <= boxCenter.x + boxSize
            && sphereCenter.x >= boxCenter.x - boxSize
            && sphereCenter.y <= boxCenter.y + boxSize
            && sphereCenter.z <= boxCenter.z + boxSize
            && sphereCenter.z >= boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, -1.0f, 0.0f));
    }
    if (sphereCenter.z - 5.0f <= boxCenter.z + boxSize
            && sphereCenter.x <= boxCenter.x + boxSize
            && sphereCenter.x >= boxCenter.x - boxSize
            && sphereCenter.y <= boxCenter.y + boxSize
            && sphereCenter.y >= boxCenter.y - boxSize
            && sphereCenter.z >= boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 0.0f, 1.0f));
    }

    if (sphereCenter.z + 5.0f >= boxCenter.z - boxSize
            && sphereCenter.x <= boxCenter.x + boxSize
            && sphereCenter.x >= boxCenter.x - boxSize
            && sphereCenter.y <= boxCenter.y + boxSize
            && sphereCenter.y >= boxCenter.y - boxSize
            && sphereCenter.z <= boxCenter.z + boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 0.0f, -1.0f));
    }
}