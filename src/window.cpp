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

    auto flipScreenKey = glfwGetKey(window, GLFW_KEY_ENTER);
    static bool sIsUp = true;
    if (flipScreenKey == GLFW_PRESS && Window::currentWindow && sIsUp) {
        Window::currentWindow->toggleFullscreen();
        sIsUp = false;
    }
    if (flipScreenKey == GLFW_RELEASE && Window::currentWindow /*&& sIsUp*/) {
        sIsUp = true;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Window::currentWindow->resize(width, height);
    glViewport(0, 0, width, height);
    Window::currentWindow->getAspectRatio();
}

void Window::resize(int width, int height) {
    if (m_fullScreen) {
        m_FullscreenWidth = width;
        m_FullscreenHeight = height;
    } else {
        m_WindowedWidth = width;
        m_WindowedHeight = height;
    }
}

void Window::toggleFullscreen() {
    if (m_fullScreen) {
        glfwSetWindowMonitor(m_window, NULL, 0, 0, m_WindowedWidth, m_WindowedHeight, 1);
        DLOGN(m_WindowedWidth, m_WindowedHeight);
        m_fullScreen = false;
    } else {
        glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, m_FullscreenWidth, m_FullscreenHeight, 1);
        DLOGN(m_FullscreenWidth, m_FullscreenHeight);
        m_fullScreen = true;
    }
}

Window::Window(int _w, int _h, const char *_name, bool _fullScreen) : m_WindowedWidth(_w), m_WindowedHeight(_h), m_fullScreen(_fullScreen) {
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

    if (_fullScreen) {
        m_window = glfwCreateWindow(1920, 1080, _name, glfwGetPrimaryMonitor(), NULL);
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

    //auto f = std::bind(&Window::framebuffer_size_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //auto f = std::bind(framebuffer_size_callback2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw("Failed to initialize GLAD");
    }
    glfwSwapInterval(1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
}

void Window::refresh() {

    currentWindow = this;
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    processInput(m_window);
    //glClearColor(0.10f, 0.1f, 0.13f, 1.0f);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float Window::getAspectRatio() {
    if (m_fullScreen)
        return static_cast<float>(m_FullscreenWidth) / static_cast<float>(m_FullscreenHeight);
    else
        return static_cast<float>(m_WindowedWidth) / static_cast<float>(m_WindowedHeight);
}

int Window::active() {
    return !glfwWindowShouldClose(m_window);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
