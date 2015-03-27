#include "myGlew.h"
#include "GLFWProgram.h"
#include "Cube.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "TextureReader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;

int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;

GLFWwindow * window;
GLProgram envProgram;

Cube * enviroment;

vec3 camPos = vec3(0.0f, 12.5f, 5.0f);
vec3 camTarget = vec3(0.0, 0.0, 10.0);
vec3 camUp = vec3(0.0, 0.0, 1.0);

mat4 model;
mat4 view;
mat4 projection;

vec4 light = vec4(0.0f, 0.0f, 5.0f, 1.0f);

GLfloat moveStep = 0.1f;

void loadEnviromentTexture()
{
    glActiveTexture(GL_TEXTURE0);

    GLuint textId;

    glGenTextures(1, &textId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textId);

    const char * names[] = { "wall", "wall", "wall", "wall", "up", "floor" };

    GLuint targets[] =
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    GLint w, h;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 512, 512);
    TextureReader textureReader;
    for (int i = 0; i < 6; i++)
    {
        std::string texName = names[i] + std::string(".tga");
        std::cout << texName << std::endl;
        GLubyte *data = textureReader.read(texName.c_str(), w, h);
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

void init()
{
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 1.0);


    enviroment = new Cube(true, 100.0f);

    GLShader envVertexShader(GLShader::GLShaderType::VERTEX);
    envVertexShader.readShader("enviroment.vert");
    envVertexShader.compileShader();

    GLShader envFragmentShader(GLShader::GLShaderType::FRAGMENT);
    envFragmentShader.readShader("enviroment.frag");
    envFragmentShader.compileShader();

    envProgram.setShaders({ envVertexShader.getId(), envFragmentShader.getId() });
    envProgram.link();

    model = mat4(1.0f);

    view = glm::lookAt(camPos, camTarget, camUp);

    projection = mat4(1.0f);
    projection = glm::perspective(glm::radians(70.0f), (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 100.0f);

    envProgram.use();
    envProgram.setUniform("light.Position", view * light);
    envProgram.setUniform("light.Intensity", 0.8f, 0.5f, 0.5f);


    envProgram.setUniform("material.Kd", 0.2f, 0.8f, 0.3f);
    envProgram.setUniform("material.Ka", 0.2f, 0.6f, 0.3f);
    envProgram.setUniform("material.Ks", 0.2f, 0.2f, 0.8f);
    envProgram.setUniform("material.Shininess", 100.0f);

    /*TextureReader textreader;
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    textreader.loadTex("wall.tga", w, h);*/

    loadEnviromentTexture();
}

void setMatrices()
{

    mat4 mv = view * model;
    envProgram.setUniform("ModelViewMatrix", mv);
    envProgram.setUniform("NormalMatrix",
                          glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

    envProgram.setUniform("MVP", projection * mv);
}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        setMatrices();
        enviroment->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void keyFunction(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    vec3 direction = camTarget - camPos;
    switch (key)
    {
    case GLFW_KEY_UP:
        camTarget.z += moveStep;
        break;
    case GLFW_KEY_DOWN:
        camTarget.z -= moveStep;
        break;
    case GLFW_KEY_LEFT:
        camTarget.x += moveStep;
        break;
    case GLFW_KEY_RIGHT:
        camTarget.x -= moveStep;
        break;
    case GLFW_KEY_W:
        camPos += direction * moveStep;
        camTarget += direction * moveStep;
        break;
    case GLFW_KEY_S:
        camPos -= direction * moveStep;
        camTarget -= direction * moveStep;
        break;
    case GLFW_KEY_A:
        camPos -= glm::cross(direction, camUp) * moveStep;
        camTarget -= glm::cross(direction, camUp) * moveStep;
        break;
    case GLFW_KEY_D:
        camPos += glm::cross(direction, camUp) * moveStep;
        camTarget += glm::cross(direction, camUp) * moveStep;
        break;
    default:
        printf("Bad key :(\n");
        break;
    }

    view = glm::lookAt(camPos, camTarget, camUp);

    glfwPollEvents();
}

int main()
{
    window = initWindow("Beadando", WIN_WIDTH, WIN_HEIGHT, true);

    init();

    glfwSetKeyCallback(window, keyFunction);

    mainLoop();

    return 0;
}