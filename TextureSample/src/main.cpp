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
#include <iostream>

#include "soil.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;

GLFWwindow * window;
GLProgram envProgram;
GLProgram sphereProgram;

Cube * enviroment;
Sphere * sphere;

GLfloat ter = 100.0f;
/*vec3 camPos = vec3(0.0f, 25.0f, 5.0f);
vec3 camTarget = vec3(0.0, 0.0, 0.0);
vec3 camUp = vec3(0.0, 0.0, 1.0);*/
Camera camera(vec3(0.0f, 5.0f, 0.0f), ter);

mat4 model;
mat4 projection;

vec4 light = vec4(-camera.getPosition(), 1.0f);

GLfloat moveStep = 0.1f;


TextureReader textureReader;

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

    int w, h, ch;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 2048, 2048);

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

void loadSphereTexture()
{
    GLint w, h;
    glActiveTexture(GL_TEXTURE0);
    textureReader.loadTex("sphereTexture.tga",w,h);
}

void init()
{
    //glEnable(GL_DEPTH_TEST);
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

    sphere = new Sphere(1.0f, 180, 180);

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

    model *= glm::translate(vec3(0.0, 0.0, -50.0));

    sphereProgram.setUniform("light.Position", camera.getView() * light);
    sphereProgram.setUniform("light.Intensity", 0.8f, 0.5f, 0.5f);


    sphereProgram.setUniform("material.Kd", 0.2f, 0.8f, 0.3f);
    sphereProgram.setUniform("material.Ka", 0.2f, 0.6f, 0.3f);
    sphereProgram.setUniform("material.Ks", 0.2f, 0.2f, 0.8f);
    sphereProgram.setUniform("material.Shininess", 100.0f);

    loadSphereTexture();

}

void setMatrices()
{

    mat4 mv = camera.getView() * mat4(1.0f);
    envProgram.setUniform("ModelViewMatrix", mv);
    envProgram.setUniform("NormalMatrix",
                          glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));

    envProgram.setUniform("MVP", projection * mv);
}

void mainLoop()
{
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        //glClear(GL_DEPTH_BUFFER_BIT);
        envProgram.use();
        setMatrices();
        enviroment->render();


        sphereProgram.use();
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

void keyFunction(GLFWwindow *window, int key, int scanCode, int action, int mods)
{

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
    case GLFW_KEY_W:
        camera.moving(Camera::Dir::W);
        break;
    case GLFW_KEY_S:
        camera.moving(Camera::Dir::S);
        break;
    case GLFW_KEY_A:
        camera.moving(Camera::Dir::A);
        break;
    case GLFW_KEY_D:
        camera.moving(Camera::Dir::D);
        break;
    default:
        printf("Bad key :(\n");
        break;
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