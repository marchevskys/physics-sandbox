#include "window.h"

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

Window::Window(int w, int h, const char *name) {
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

    if (false) {
        m_window = glfwCreateWindow(1920, 1080, "name", glfwGetPrimaryMonitor(),
                                    NULL);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        m_window = glfwCreateWindow(w, h, "LearnOpenGL", NULL, NULL);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (m_window == nullptr) {
        glfwTerminate();
        throw("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowTitle(m_window, name);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int Window::active() {
    return !glfwWindowShouldClose(m_window);
}

Window::~Window() {
    glfwTerminate();
}
