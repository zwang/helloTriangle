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

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static float color = 1;
static double cursorCurrentX = 0;
static double cursorCurrentY = 0;
static ContextPtr ctxPtr;

GLFWwindow *getGlfwWindow();

int runGLFWWindow();

void setCallbacks(GLFWwindow *window);

void loadAndInitGame();

void drawWindow(GLFWwindow *window);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        color = (float) rand()/RAND_MAX;
    } else if (key == GLFW_KEY_0 && action == GLFW_PRESS && !secondWindow) {
        secondWindow = glfwCreateWindow(640, 480, "Click Demo", nullptr, nullptr);
        if (!secondWindow) {
            std::cerr << "Could not create second window\n";
        }
        glfwSetKeyCallback(secondWindow, key_callback);
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::string s2 = "onClick("+ std::to_string(cursorCurrentX) + "," +std::to_string(cursorCurrentY) + ")";
        RunScript(ctxPtr, s2.c_str(), "main.js");
    }
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    cursorCurrentX = xpos;
    cursorCurrentY = ypos;
}

ContextPtr loadAndInitGameView() {
    IsolatePtr iso = NewIsolate();
    ContextPtr ctxP = NewContext(iso, nullptr, 1);
    string filePath = R"(C:\Users\zhaow\source\helloTriangle\bin\js\touchDemo\main.js)";
    ifstream ifs(filePath);
    string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()) );
    RunScript(ctxP, content.c_str(), "main.js");
    RunScript(ctxP, "initSurface();", "main.js");
    return ctxP;
}

void setCallbacks(GLFWwindow *window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
}

GLFWwindow *getGlfwWindow() {
    GLFWwindow* window;
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(640, 480, "Click 1 To Change Color", nullptr, nullptr);
    return window;
}

void drawWindow(GLFWwindow *window, ContextPtr ctxP) {
    glfwMakeContextCurrent(window);
    RunScript(ctxP, "drawFrame();", "main.js");
    glfwSwapBuffers(window);
}

int main() {
    return runGLFWWindow();
}

int runGLFWWindow() {
    srand((unsigned)time(nullptr ) );
    if (!glfwInit()) {
        cerr << "Could not init GLFW\n";
        return 1;
    }
    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = getGlfwWindow();

    if (!window) {
        cerr << "Could not create window\n";
        glfwTerminate();
        return -1;
    }

    setCallbacks(window);

    glfwMakeContextCurrent(window);

    Init();
    ctxPtr = loadAndInitGameView();

    // rest of code goes here
    while (!glfwWindowShouldClose(window)) {
        drawWindow(window, ctxPtr);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


