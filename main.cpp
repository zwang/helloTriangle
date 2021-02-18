#define GLFW_INCLUDE_NONE
#include "windows.h"
#include <iostream>
#include <time.h>
#include <GLFW/glfw3.h>
#include "v8go.h"
#include <cstdio>
#include <fstream>
#include "v8.h"
using namespace v8;
using namespace std;

static GLFWwindow* secondWindow;

bool sonic_gl_error_check = true;

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
        {
                { -0.6f, -0.4f, 1.f, 0.f, 0.f },
                {  0.6f, -0.4f, 0.f, 1.f, 0.f },
                {   0.f,  0.6f, 0.f, 0.f, 1.f }
        };

static const char* vertex_shader_text =
        "#version 110\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragment_shader_text =
        "#version 110\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static float color = 1;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        color = (float) rand()/RAND_MAX;
    } else if (key == GLFW_KEY_0 && action == GLFW_PRESS && !secondWindow) {
        secondWindow = glfwCreateWindow(640, 480, "Click 1 To Change Color", nullptr, nullptr);
        if (!secondWindow) {
            std::cerr << "Could not create second window\n";
        }
        glfwSetKeyCallback(secondWindow, key_callback);
    }
}

int main() {
    srand( (unsigned)time( nullptr ) );
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Could not init GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(640, 480, "Click 1 To Change Color", nullptr, nullptr);
    if (!window) {
        std::cerr << "Could not create window\n";
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    Init();
    IsolatePtr iso = NewIsolate();
    ContextPtr ctxPtr = NewContext(iso, nullptr, 1);
    ValuePtr global = ContextGlobal(ctxPtr);

    std::string filePath = R"(C:\Users\zhaow\source\helloTriangle\bin\js\touchDemo\main.js)";

    std::ifstream ifs(filePath);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    RunScript(ctxPtr, content.c_str(), "main.js");
    RunScript(ctxPtr, "initSurface();", "main.js");

    // rest of code goes here
    while (!glfwWindowShouldClose(window)) {
        glfwMakeContextCurrent(window);
        //std::string s1 = "GL.clearColor(0, 0, " + std::to_string(color) + ", 1);";
        //RunScript(ctxPtr, s1.c_str(), "demo.js");
        //RunScript(ctxPtr, "GL.clear(GL.COLOR_BUFFER_BIT);", "demo.js");
        RunScript(ctxPtr, "drawFrame();", "main.js");
        // glClearColor(0, 0, 1, 1);
        // glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        if (secondWindow != nullptr && !glfwWindowShouldClose(secondWindow)) {
            glfwMakeContextCurrent(secondWindow);
            std::string s2 = "GL.clearColor("+ std::to_string(color)+ ", 0, 0, 1);";
            RunScript(ctxPtr, s2.c_str(), "demo.js");
            RunScript(ctxPtr, "GL.clear(GL.COLOR_BUFFER_BIT);", "demo.js");
            // glClearColor(0, 0, 1, 1);
            // glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(secondWindow);
        }

        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
