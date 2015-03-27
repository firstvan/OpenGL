#pragma once

#include "myGlew.h"
#include <GLFW\glfw3.h>
#include <string>



GLFWwindow* initWindow(std::string s,int width,int height, bool fullScreen)
{
    GLFWwindow* window;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    if (fullScreen)
    {
        window = glfwCreateWindow(width, height, s.c_str(), glfwGetPrimaryMonitor(), nullptr);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else
        window = glfwCreateWindow(width, height, s.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}

void DrawFps(int& nbFrames,double& lastTime)
{

    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0)  // If last prinf() was more than 1 sec ago
    {
        // printf and reset timer
        printf("%.1f fps\n", nbFrames / (currentTime - lastTime));
        nbFrames = 0;
        lastTime += 1.0;
    }


}