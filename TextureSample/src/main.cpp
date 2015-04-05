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

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;
bool needInit = true;
double last_x = 0;
double last_y =  0;

GLFWwindow * window;
GLProgram envProgram;
GLProgram sphereProgram;
GLProgram boxProgram;

Cube * enviroment;
Sphere * sphere;
Cube * box;

GLfloat ter = 50.0f;
Camera camera(vec3(-20.0f, 20.0f, 20.0f), ter);

mat4 model;
mat4 projection;

vec4 light = vec4(-camera.getPosition(), 1.0f);
vec4 lightPos;
GLfloat moveStep = 0.1f;

TextureReader textureReader;

GLuint sphereTextureColorBind;
GLuint sphereTextureTransBind;

GLuint envTextureBind;
GLuint boxTextureBind;


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
    sphereProgram.setUniform("viewPos", camera.getPosition());
    sphereProgram.setUniform("material.shininess", 100.0f);
    sphereProgram.setUniform("material.specular", vec3(1.0f, 1.0f, 1.0f));

    sphereProgram.setUniform("spotLight[0].position", enviroment->getVertex(20));
    sphereProgram.setUniform("spotLight[0].direction", vec3(0.0f, -5.0f, 0.0f));
    sphereProgram.setUniform("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[0].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[0].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[0].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[1].position", enviroment->getVertex(21));
    sphereProgram.setUniform("spotLight[1].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[1].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[1].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[1].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[1].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[1].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[2].position", enviroment->getVertex(22));
    sphereProgram.setUniform("spotLight[2].direction", vec3(0.0f, -5.0f, 0.0f));
    sphereProgram.setUniform("spotLight[2].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[2].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[2].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[2].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[2].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[3].position", enviroment->getVertex(23));
    sphereProgram.setUniform("spotLight[3].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[3].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[3].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[3].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[3].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[3].specular", vec3(1.0f));

    sphereProgram.setUniform("pointLights[0].position", vec3(-10.0f, ter, -10.0f));
    sphereProgram.setUniform("pointLights[0].ambient", vec3(0.2f));
    sphereProgram.setUniform("pointLights[0].diffuse", vec3(0.8f));
    sphereProgram.setUniform("pointLights[0].specular", vec3(1.0f));

    sphereProgram.setUniform("pointLights[1].position", vec3(10.0f, ter, 10.0f));
    sphereProgram.setUniform("pointLights[1].ambient", vec3(0.2f));
    sphereProgram.setUniform("pointLights[1].diffuse", vec3(0.8f));
    sphereProgram.setUniform("pointLights[1].specular", vec3(1.0f));
}

void setLights2()
{
    boxProgram.setUniform("viewPos", camera.getPosition());
    boxProgram.setUniform("material.shininess", 100.0f);
    boxProgram.setUniform("material.specular", vec3(1.0f, 1.0f, 1.0f));

    boxProgram.setUniform("spotLight[0].position", enviroment->getVertex(20));
    boxProgram.setUniform("spotLight[0].direction", vec3(0.0f, -5.0f, 0.0f));
    boxProgram.setUniform("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
    boxProgram.setUniform("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
    boxProgram.setUniform("spotLight[0].ambient", vec3(0.2f));
    boxProgram.setUniform("spotLight[0].diffuse", vec3(0.8f));
    boxProgram.setUniform("spotLight[0].specular", vec3(1.0f));

    boxProgram.setUniform("spotLight[1].position", enviroment->getVertex(21));
    boxProgram.setUniform("spotLight[1].direction", vec3(0.0f, -1.0f, 0.0f));
    boxProgram.setUniform("spotLight[1].cutOff", glm::cos(glm::radians(10.0f)));
    boxProgram.setUniform("spotLight[1].outerCutOff", glm::cos(glm::radians(20.0f)));
    boxProgram.setUniform("spotLight[1].ambient", vec3(0.2f));
    boxProgram.setUniform("spotLight[1].diffuse", vec3(0.8f));
    boxProgram.setUniform("spotLight[1].specular", vec3(1.0f));

    boxProgram.setUniform("spotLight[2].position", enviroment->getVertex(22));
    boxProgram.setUniform("spotLight[2].direction", vec3(0.0f, -5.0f, 0.0f));
    boxProgram.setUniform("spotLight[2].cutOff", glm::cos(glm::radians(10.0f)));
    boxProgram.setUniform("spotLight[2].outerCutOff", glm::cos(glm::radians(20.0f)));
    boxProgram.setUniform("spotLight[2].ambient", vec3(0.2f));
    boxProgram.setUniform("spotLight[2].diffuse", vec3(0.8f));
    boxProgram.setUniform("spotLight[2].specular", vec3(1.0f));

    boxProgram.setUniform("spotLight[3].position", enviroment->getVertex(23));
    boxProgram.setUniform("spotLight[3].direction", vec3(0.0f, -1.0f, 0.0f));
    boxProgram.setUniform("spotLight[3].cutOff", glm::cos(glm::radians(10.0f)));
    boxProgram.setUniform("spotLight[3].outerCutOff", glm::cos(glm::radians(20.0f)));
    boxProgram.setUniform("spotLight[3].ambient", vec3(0.2f));
    boxProgram.setUniform("spotLight[3].diffuse", vec3(0.8f));
    boxProgram.setUniform("spotLight[3].specular", vec3(1.0f));

    boxProgram.setUniform("pointLights[0].position", vec3(-10.0f, ter, -10.0f));
    boxProgram.setUniform("pointLights[0].ambient", vec3(0.2f));
    boxProgram.setUniform("pointLights[0].diffuse", vec3(0.8f));
    boxProgram.setUniform("pointLights[0].specular", vec3(1.0f));

    boxProgram.setUniform("pointLights[1].position", vec3(10.0f, ter, 10.0f));
    boxProgram.setUniform("pointLights[1].ambient", vec3(0.2f));
    boxProgram.setUniform("pointLights[1].diffuse", vec3(0.8f));
    boxProgram.setUniform("pointLights[1].specular", vec3(1.0f));
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


    enviroment = new Cube(true, ter * 2);

    GLShader envVertexShader(GLShader::GLShaderType::VERTEX);
    envVertexShader.readShader("enviroment.vert");
    envVertexShader.compileShader();

    GLShader envFragmentShader(GLShader::GLShaderType::FRAGMENT);
    envFragmentShader.readShader("enviroment.frag");
    envFragmentShader.compileShader();

    envProgram.setShaders({ envVertexShader.getId(), envFragmentShader.getId() });
    envProgram.link();

    projection = mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / WIN_HEIGHT, 0.05f, 500.0f);

    envProgram.use();

    envTextureBind = loadEnviromentTexture();

    sphere = new Sphere(5.0f, 180, 180);
    sphere->dir = vec3(1.0f, 0.0f, 0.0f);

    GLShader sphereVertexShader(GLShader::GLShaderType::VERTEX);
    sphereVertexShader.readShader("sphere.vert");
    sphereVertexShader.compileShader();

    GLShader sphereFragmentShader(GLShader::GLShaderType::FRAGMENT);
    sphereFragmentShader.readShader("sphere.frag");
    sphereFragmentShader.compileShader();

    sphereProgram.setShaders({ sphereVertexShader.getId(), sphereFragmentShader.getId() });
    sphereProgram.link();

    sphereProgram.use();

    model = mat4(1.0f);

    setLights();

    loadSphereTexture();

    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));

    box = new Cube(false, boxSize * 2);

    GLShader boxVertexShader(GLShader::GLShaderType::VERTEX);
    boxVertexShader.readShader("box.vert");
    boxVertexShader.compileShader();

    GLShader boxFragmentShader(GLShader::GLShaderType::FRAGMENT);
    boxFragmentShader.readShader("box.frag");
    boxFragmentShader.compileShader();

    boxProgram.setShaders({ boxVertexShader.getId(), boxFragmentShader.getId() });
    boxProgram.link();

    boxProgram.use();
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    boxTextureBind = textureReader.loadTex("box.tga", w, h);
    setLights2();

    boxCenter = vec3(30.0f, 0.0f, 0.0f);
    movedBox = mat4(1.0f);
    movedBox = glm::translate(boxCenter);
}

void setMatrices()
{

    mat4 mv = camera.getView() * mat4(1.0f);
    envProgram.setUniform("ModelViewMatrix", mv);
    envProgram.setUniform("NormalMatrix",
                          glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    mat4 mvp = projection * mv;
    envProgram.setUniform("MVP", mvp);
}

void moveCamera();
void moveBox();

vec3 sphereCenter(0.0f, 0.0f, 0.0f);
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
            && sphereCenter.x > boxCenter.x - boxSize
            && sphereCenter.y < boxCenter.y + boxSize
            && sphereCenter.y > boxCenter.y - boxSize
            && sphereCenter.z < boxCenter.z + boxSize
            && sphereCenter.z > boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(1.0f, 0.0f, 0.0f));
        std::cout << "+x" <<  std::endl;
    }

    if (sphereCenter.x + 5.0f >= boxCenter.x - boxSize
            && sphereCenter.x < boxCenter.x + boxSize
            && sphereCenter.y < boxCenter.y + boxSize
            && sphereCenter.y > boxCenter.y - boxSize
            && sphereCenter.z < boxCenter.z + boxSize
            && sphereCenter.z > boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(-1.0f, 0.0f, 0.0f));
        std::cout << "-x sphCent(" << sphereCenter.x << ", " << sphereCenter.y << ", " << sphereCenter.z<< ")"<<std::endl;
    }

    if (sphereCenter.y - 5.0f <= boxCenter.y + boxSize
            && sphereCenter.x > boxCenter.x - boxSize
            && sphereCenter.x < boxCenter.x + boxSize
            && sphereCenter.y > boxCenter.y - boxSize
            && sphereCenter.z < boxCenter.z + boxSize
            && sphereCenter.z > boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 1.0f, 0.0f));
        std::cout << "+y" << std::endl;
    }

    if (sphereCenter.y + 5.0f >= boxCenter.y - boxSize
            && sphereCenter.x < boxCenter.x + boxSize
            && sphereCenter.x > boxCenter.x - boxSize
            && sphereCenter.y < boxCenter.y + boxSize
            && sphereCenter.z < boxCenter.z + boxSize
            && sphereCenter.z > boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, -1.0f, 0.0f));
        std::cout << "-y" << std::endl;
    }
    if (sphereCenter.z - 5.0f <= boxCenter.z + boxSize
            && sphereCenter.x < boxCenter.x + boxSize
            && sphereCenter.x > boxCenter.x - boxSize
            && sphereCenter.y < boxCenter.y + boxSize
            && sphereCenter.y > boxCenter.y - boxSize
            && sphereCenter.z > boxCenter.z - boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 0.0f, 1.0f));
        std::cout << "+z" << std::endl;

    }

    if (sphereCenter.z + 5.0f >= boxCenter.z - boxSize
            && sphereCenter.x < boxCenter.x + boxSize
            && sphereCenter.x > boxCenter.x - boxSize
            && sphereCenter.y < boxCenter.y + boxSize
            && sphereCenter.y > boxCenter.y - boxSize
            && sphereCenter.z < boxCenter.z + boxSize)
    {
        sphere->dir = glm::reflect(sphere->dir, vec3(0.0f, 0.0f, -1.0f));
        std::cout << "-z" << std::endl;
    }
}


void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        moveCamera();
        moveBox();

        envProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, envTextureBind);
        setMatrices();
        enviroment->render();


        sphereProgram.use();

        envReflect();
        boxReflect();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sphereTextureColorBind);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sphereTextureTransBind);


        vec3 d = sphere->dir * 0.1f;
        sphereCenter += d;
        model *= glm::translate(d);
        mat4 mv = camera.getView() * model;
        sphereProgram.setUniform("ModelViewMatrix", mv);
        sphereProgram.setUniform("NormalMatrix",
                                 glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

        sphereProgram.setUniform("MVP", projection * mv);
        sphere->render();

        boxProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTextureBind);

        mv = camera.getView() * movedBox;
        envProgram.setUniform("ModelViewMatrix", mv);
        envProgram.setUniform("NormalMatrix",
                              glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
        mat4 mvp = projection * mv;
        envProgram.setUniform("MVP", mvp);

        box->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void moveCamera()
{
    if (keyStates[GLFW_KEY_W])
    {
        camera.moving(Camera::Dir::W);
    }

    if (keyStates[GLFW_KEY_S])
    {
        camera.moving(Camera::Dir::S);
    }

    if (keyStates[GLFW_KEY_A])
    {
        camera.moving(Camera::Dir::A);
    }

    if (keyStates[GLFW_KEY_D])
    {
        camera.moving(Camera::Dir::D);
    }

    if (keyStates[GLFW_KEY_SPACE])
    {
        camera.moving(Camera::Dir::UP);
    }

    if (keyStates[GLFW_KEY_LEFT_CONTROL])
    {
        camera.moving(Camera::Dir::DOWN);
    }
    if (keyStates[GLFW_KEY_LEFT_SHIFT])
    {
        camera.setSprint(5.0);
    }
    if (!keyStates[GLFW_KEY_LEFT_SHIFT])
    {
        camera.setSprint(1.0);
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

    switch (key)
    {
    case GLFW_KEY_UP:
        camera.rotate(0.0f, -1.0f);
        break;
    case GLFW_KEY_DOWN:
        camera.rotate(0.0f, 1.0f);
        break;
    case GLFW_KEY_LEFT:
        camera.rotate(1.0f, 0.0f);
        break;
    case GLFW_KEY_RIGHT:
        camera.rotate(-1.0f, 0.0f);
        break;
    default:
        break;
    }



    glfwPollEvents();
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    if (needInit)
    {
        last_x = xpos;
        last_y = ypos;
        needInit = false;
    }

    double rotate_x = xpos - last_x;
    double rotate_y = ypos - last_y;
    last_x = xpos;
    last_y = ypos;
    camera.rotate(rotate_x, rotate_y);
}

int main()
{
    window = initWindow("Beadando", WIN_WIDTH, WIN_HEIGHT, false);

    init();

    glfwSetKeyCallback(window, keyFunction);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mainLoop();

    return 0;
}