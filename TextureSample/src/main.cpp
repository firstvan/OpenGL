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
vec3 camPos = vec3( - ter + 3, ter - 3, -ter + 3);
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
std::vector<vec3> boxSides;
float boxSize = 15.0f;
float sS = 1.0f;

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

    //glm::mat3 normalMatrix = glm::mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    //prog.setUniform("lights[0].position", vec3(glm::normalize(view * vec4(vec3(ter, ter, ter), 1.0f))));
    prog.setUniform("lights[0].position", vec3(ter, ter, ter));
    prog.setUniform("lights[0].cutOff", glm::cos(glm::radians(12.5f)));
    prog.setUniform("lights[0].outerCutOff", glm::cos(glm::radians(17.5f)));
    prog.setUniform("lights[0].ambient", 0.0f, 0.0f, 0.0f);
    prog.setUniform("lights[0].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("lights[0].specular", 1.0f, 1.0f, 1.0f);

    //prog.setUniform("lights[1].position", vec3(glm::normalize(view * vec4(vec3(ter, ter, -ter), 1.0f))));
    prog.setUniform("lights[1].position", vec3(ter, ter, -ter));
    prog.setUniform("lights[1].cutOff", glm::cos(glm::radians(12.5f)));
    prog.setUniform("lights[1].outerCutOff", glm::cos(glm::radians(17.5f)));
    prog.setUniform("lights[1].ambient", 0.0f, 0.0f, 0.0f);
    prog.setUniform("lights[1].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("lights[1].specular", 1.0f, 1.0f, 1.0f);

    //prog.setUniform("lights[2].position", vec3(glm::normalize(view * vec4(vec3(-ter, ter, ter), 1.0f))));
    prog.setUniform("lights[2].position", vec3(-ter, ter, ter));
    prog.setUniform("lights[2].cutOff", glm::cos(glm::radians(12.5f)));
    prog.setUniform("lights[2].outerCutOff", glm::cos(glm::radians(17.5f)));
    prog.setUniform("lights[2].ambient", 0.0f, 0.0f, 0.0f);
    prog.setUniform("lights[2].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("lights[2].specular", 1.0f, 1.0f, 1.0f);

    //prog.setUniform("lights[3].position", vec3(glm::normalize(view * vec4(vec3(-ter, ter, -ter), 1.0f))));
    prog.setUniform("lights[3].position", vec3(-ter, ter, -ter));
    prog.setUniform("lights[3].cutOff", glm::cos(glm::radians(12.5f)));
    prog.setUniform("lights[3].outerCutOff", glm::cos(glm::radians(17.5f)));
    prog.setUniform("lights[3].ambient", 0.0f, 0.0f, 0.0f);
    prog.setUniform("lights[3].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("lights[3].specular", 1.0f, 1.0f, 1.0f);

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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
    projection = glm::perspective(glm::radians(50.0f), (float)WIN_WIDTH / WIN_HEIGHT, 0.01f, 500.0f);

    envTextureBind = loadEnviromentTexture();

    sphere = new Sphere(sS, 180, 180);
    sphere->dir = vec3(1.0f, 1.0f, 0.5f);

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

    boxCenter = vec3(10.0f, 0.0f, 0.0f);
    movedBox = mat4(1.0f);
    movedBox = glm::translate(boxCenter);

}

void setMatrices()
{

    prog.setUniform("Model", mat4(1.0f));
    prog.setUniform("NormalMatrix",
                    glm::mat3(glm::transpose(glm::inverse(mat4(1.0f)))));
    mat4 mvp = projection * view * mat4(1.0f);
    prog.setUniform("MVP", mvp);
}


void moveBox();

void envReflect();

void boxReflect();

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        moveBox();
        view = glm::lookAt(camPos, sphereCenter, camUp);

        prog.setUniform("lights[0].direction", glm::normalize(boxCenter - vec3(ter, ter, ter)));
        prog.setUniform("lights[1].direction", glm::normalize(boxCenter - vec3(ter, ter, -ter)));
        prog.setUniform("lights[2].direction", glm::normalize(boxCenter - vec3(-ter, ter, ter)));
        prog.setUniform("lights[3].direction", glm::normalize(boxCenter - vec3(-ter, ter, -ter)));

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


        vec3 d = sphere->dir * 1.0f;
        sphereCenter += d;
        model *= glm::translate(d);
        prog.setUniform("Model", model);
        prog.setUniform("NormalMatrix",
                        glm::mat3(glm::transpose(glm::inverse(model))));

        mat4 mv = view * model;
        prog.setUniform("MVP", projection * mv);
        prog.setUniform("needMix", true);
        sphere->render();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTextureBind);


        prog.setUniform("Model", movedBox);
        prog.setUniform("NormalMatrix",
                        glm::mat3(glm::transpose(glm::inverse(movedBox))));
        mat4 mvp = projection * view * movedBox;
        prog.setUniform("MVP", mvp);
        prog.setUniform("needMix", false);
        box->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void moveBox()
{
    if (keyStates[GLFW_KEY_W])
    {
        if (boxCenter.z + boxSize < ter)
        {
            boxCenter.z += 0.5;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_S])
    {
        if (boxCenter.z - boxSize > -ter)
        {
            boxCenter.z -= 0.5;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_A])
    {
        if (boxCenter.x + boxSize < ter)
        {
            boxCenter.x += 0.5;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_D])
    {
        if (boxCenter.x - boxSize > -ter)
        {
            boxCenter.x -= 0.5;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_SPACE])
    {
        if (boxCenter.y + boxSize < ter)
        {
            boxCenter.y += 0.5;
            movedBox = glm::translate(boxCenter);
        }
    }

    if (keyStates[GLFW_KEY_LEFT_CONTROL])
    {
        if (boxCenter.y - boxSize > -ter)
        {
            boxCenter.y -= 0.5;
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
    if (sphereCenter.x > ter - sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[3]);
    }

    if (sphereCenter.x <  -ter + sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[1]);
    }

    if (sphereCenter.z > ter - sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[0]);
    }

    if (sphereCenter.z <  -ter + sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[2]);
    }

    if (sphereCenter.y > ter - sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[5]);
    }

    if (sphereCenter.y <  -ter + sS)
    {
        sphere->dir = glm::reflect(sphere->dir, normals[4]);
    }

}


GLfloat distPlanPoint(vec3 point, vec3 planePoint, vec3 normal)
{
    GLfloat D = planePoint.x * normal.x + planePoint.y * normal.y + planePoint.z * normal.z;
    return (normal.x * point.x + normal.y * point.y + normal.z * point.z - D);
}

void boxReflect()
{
    vec3 posx = vec3(boxCenter.x + boxSize, boxCenter.y, boxCenter.z);
    vec3 posxn = vec3(1.0f, 0.0f, 0.0f);
    vec3 negx = vec3(boxCenter.x - boxSize, boxCenter.y, boxCenter.z);
    vec3 negxn = vec3(-1.0f, 0.0f, 0.0f);

    vec3 posy = vec3(boxCenter.x, boxCenter.y + boxSize, boxCenter.z);
    vec3 posyn = vec3(0.0f, 1.0f, 0.0f);
    vec3 negy = vec3(boxCenter.x, boxCenter.y - boxSize, boxCenter.z);
    vec3 negyn = vec3(0.0f, -1.0f, 0.0f);


    vec3 posz = vec3(boxCenter.x, boxCenter.y, boxCenter.z + boxSize);
    vec3 poszn = vec3(0.0f, 0.0f, 1.0f);
    vec3 negz = vec3(boxCenter.x, boxCenter.y, boxCenter.z - boxSize);
    vec3 negzn = vec3(0.0f, 0.0f, -1.0f);

    if (glm::abs(distPlanPoint(sphereCenter, posx, posxn)) <= sS &&
            distPlanPoint(boxCenter, posx, posxn) * distPlanPoint(sphereCenter, posx, posxn) < 0 &&
            posy.y >= sphereCenter.y && negy.y < sphereCenter.y &&
            posz.z >= sphereCenter.z && negz.z < sphereCenter.z
       )
    {
        sphere->dir = glm::reflect(sphere->dir, posxn);
    }

    if (glm::abs(distPlanPoint(sphereCenter, negx, negxn)) <= sS &&
            distPlanPoint(boxCenter, negx, negxn) * distPlanPoint(sphereCenter, negx, negxn) < 0 &&
            posy.y >= sphereCenter.y && negy.y < sphereCenter.y &&
            posz.z >= sphereCenter.z && negz.z < sphereCenter.z
       )
    {
        sphere->dir = glm::reflect(sphere->dir, negxn);
    }

    if (glm::abs(distPlanPoint(sphereCenter, posy, posyn)) <= sS &&
            distPlanPoint(boxCenter, posy, posyn) * distPlanPoint(sphereCenter, posy, posyn) < 0 &&
            posx.x >= sphereCenter.x && negx.x < sphereCenter.x &&
            posz.z >= sphereCenter.z && negz.z < sphereCenter.z
       )
    {
        sphere->dir = glm::reflect(sphere->dir, posyn);
    }

    if (glm::abs(distPlanPoint(sphereCenter, negy, negyn)) <= sS &&
            distPlanPoint(boxCenter, negy, negyn) * distPlanPoint(sphereCenter, negy, negyn) < 0 &&
            posx.x >= sphereCenter.x && negx.x < sphereCenter.x &&
            posz.z >= sphereCenter.z && negz.z < sphereCenter.z
       )
    {
        sphere->dir = glm::reflect(sphere->dir, negyn);
    }

    if (glm::abs(distPlanPoint(sphereCenter, posz, poszn)) <= sS &&
            distPlanPoint(boxCenter, posz, poszn) * distPlanPoint(sphereCenter, posz, poszn) < 0 &&
            posy.y >= sphereCenter.y && negy.y < sphereCenter.y &&
            posx.x >= sphereCenter.x && negx.x < sphereCenter.x
       )
    {
        sphere->dir = glm::reflect(sphere->dir, poszn);
    }

    if (glm::abs(distPlanPoint(sphereCenter, negz, negzn)) <= sS &&
            distPlanPoint(boxCenter, negz, negzn) * distPlanPoint(sphereCenter, negz, negzn) < 0 &&
            posy.y >= sphereCenter.y && negy.y < sphereCenter.y &&
            posx.x >= sphereCenter.x && negx.x < sphereCenter.x
       )
    {
        sphere->dir = glm::reflect(sphere->dir, negzn);
    }

}