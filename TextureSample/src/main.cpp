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

int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;
bool needInit = true;
double last_x = 0;
double last_y =  0;

GLFWwindow * window;
GLProgram envProgram;
GLProgram sphereProgram;

Cube * enviroment;
Sphere * sphere;

GLfloat ter = 50.0f;
Camera camera(vec3(-20.0f, 20.0f, 20.0f), ter);

mat4 model;
mat4 projection;

vec4 light = vec4(-camera.getPosition(), 1.0f);
vec4 lightPos;
GLfloat moveStep = 0.1f;

TextureReader textureReader;
GLuint sphereTextureBind;

GLuint keyStates[256];
std::vector<vec3> normals;

void loadEnviromentTexture()
{
    glActiveTexture(GL_TEXTURE0);

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    const char * names[] = { "posx", "negx", "posy", "negy", "posz", "negz" };

    GLuint targets[] =
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    int w, h;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 2048, 2048);

    for (int i = 0; i < 6; i++)
    {
        std::string texName = names[i] + std::string(".tga");
        std::cout << texName << std::endl;
        unsigned char *data = textureReader.read(texName.c_str(), w, h);
        glTexSubImage2D(targets[i], 0, 0, 0, w, h,
                        GL_RGBA, GL_UNSIGNED_BYTE, data);
        delete[] data;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                    GL_CLAMP_TO_EDGE);


}

GLuint loadSphereTexture()
{
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    return textureReader.loadTex("sphereTexture.tga",w,h);
}

void setLights()
{
    sphereProgram.setUniform("viewPos", camera.getPosition());
    sphereProgram.setUniform("material.shininess", 0.0f);

    sphereProgram.setUniform("spotLight[0].position", enviroment->getVertex(20));
    sphereProgram.setUniform("spotLight[0].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[0].constant", 1.0f);
    sphereProgram.setUniform("spotLight[0].linear", 0.09f);
    sphereProgram.setUniform("spotLight[0].quadratic", 0.032f);
    sphereProgram.setUniform("spotLight[0].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[0].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[0].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[1].position", enviroment->getVertex(21));
    sphereProgram.setUniform("spotLight[1].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[1].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[1].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[1].constant", 1.0f);
    sphereProgram.setUniform("spotLight[1].linear", 0.09f);
    sphereProgram.setUniform("spotLight[1].quadratic", 0.032f);
    sphereProgram.setUniform("spotLight[1].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[1].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[1].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[2].position", enviroment->getVertex(22));
    sphereProgram.setUniform("spotLight[2].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[2].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[2].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[2].constant", 1.0f);
    sphereProgram.setUniform("spotLight[2].linear", 0.09f);
    sphereProgram.setUniform("spotLight[2].quadratic", 0.032f);
    sphereProgram.setUniform("spotLight[2].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[2].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[2].specular", vec3(1.0f));

    sphereProgram.setUniform("spotLight[3].position", enviroment->getVertex(23));
    sphereProgram.setUniform("spotLight[3].direction", vec3(0.0f, -1.0f, 0.0f));
    sphereProgram.setUniform("spotLight[3].cutOff", glm::cos(glm::radians(10.0f)));
    sphereProgram.setUniform("spotLight[3].outerCutOff", glm::cos(glm::radians(20.0f)));
    sphereProgram.setUniform("spotLight[3].constant", 1.0f);
    sphereProgram.setUniform("spotLight[3].linear", 0.09f);
    sphereProgram.setUniform("spotLight[3].quadratic", 0.032f);
    sphereProgram.setUniform("spotLight[3].ambient", vec3(0.2f));
    sphereProgram.setUniform("spotLight[3].diffuse", vec3(0.8f));
    sphereProgram.setUniform("spotLight[3].specular", vec3(1.0f));


    sphereProgram.setUniform("pointLights[0].position", vec3(-10.0f, ter, -10.0f));
    sphereProgram.setUniform("pointLights[0].constant", 1.0f);
    sphereProgram.setUniform("pointLights[0].linear", 0.09f);
    sphereProgram.setUniform("pointLights[0].quadratic", 0.032f);
    sphereProgram.setUniform("pointLights[0].ambient", vec3(0.2f));
    sphereProgram.setUniform("pointLights[0].diffuse", vec3(0.8f));
    sphereProgram.setUniform("pointLights[0].specular", vec3(1.0f));

    sphereProgram.setUniform("pointLights[1].position", vec3(10.0f, ter, 10.0f));
    sphereProgram.setUniform("pointLights[1].constant", 1.0f);
    sphereProgram.setUniform("pointLights[1].linear", 0.09f);
    sphereProgram.setUniform("pointLights[1].quadratic", 0.032f);
    sphereProgram.setUniform("pointLights[1].ambient", vec3(0.2f));
    sphereProgram.setUniform("pointLights[1].diffuse", vec3(0.8f));
    sphereProgram.setUniform("pointLights[1].specular", vec3(1.0f));
}

void init()
{
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

    loadEnviromentTexture();

    sphere = new Sphere(5.0f, 180, 180);

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

    sphereTextureBind = loadSphereTexture();

    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));
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

vec3 xPlaneVec(ter, 0.0f, 0.0f);
vec3 sphereCenter(0.0f, 0.0f, 0.0f);

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        moveCamera();

        envProgram.use();
        setMatrices();
        enviroment->render();


        sphereProgram.use();

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

        vec3 d = sphere->dir * 0.1f;
        sphereCenter += d;
        model *= glm::translate(d);
        mat4 mv = camera.getView() * model;
        sphereProgram.setUniform("ModelViewMatrix", mv);
        sphereProgram.setUniform("NormalMatrix",
                                 glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

        sphereProgram.setUniform("MVP", projection * mv);
        sphere->render();


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
        camera.setSprint(2.0);
    }
    if (!keyStates[GLFW_KEY_LEFT_SHIFT])
    {
        camera.setSprint(1.0);
    }
}

void keyFunction(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keyStates[key] = 1;
    }
    else if (action == GLFW_RELEASE)
    {
        keyStates[key] = 0;
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
    window = initWindow("Beadando", WIN_WIDTH, WIN_HEIGHT, true);

    init();

    glfwSetKeyCallback(window, keyFunction);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mainLoop();

    return 0;
}