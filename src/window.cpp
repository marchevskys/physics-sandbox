#include "window.h"
#include "logger.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
Window *Window::currentWindow = nullptr;
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    Window::currentWindow->resize(width, height);
    glViewport(0, 0, width, height);
}

void Window::resize(int width, int height) {
    std::cout << "res" << std::endl;
    m_width = width;
    m_height = height;
}

Window::Window(int _w, int _h, const char *_name, bool _fillScreen) : m_width(_w), m_height(_h) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    if (_fillScreen) {
        m_window = glfwCreateWindow(1920, 1080, _name, glfwGetPrimaryMonitor(),
                                    NULL);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        m_window = glfwCreateWindow(_w, _h, _name, NULL, NULL);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (m_window == nullptr) {
        glfwTerminate();
        throw("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    //glfwSetWindowTitle(m_window, _name);
    currentWindow = this;
    //auto f = std::bind(&Window::framebuffer_size_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //auto f = std::bind(framebuffer_size_callback2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw("Failed to initialize GLAD");
    }
}

void Window::refresh() {

    glfwSwapBuffers(m_window);
    glfwPollEvents();
    processInput(m_window);
    glClearColor(0.10f, 0.1f, 0.13f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE /*GL_FILL*/);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float Window::getAspectRatio() {
    return static_cast<float>(m_width) / static_cast<float>(m_height);
    DLOGN(m_width);
}

int Window::active() {
    return !glfwWindowShouldClose(m_window);
}

Window::~Window() {
    glfwTerminate();
}
