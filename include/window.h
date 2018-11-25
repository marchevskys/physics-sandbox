#ifndef WINDOW_H
#define WINDOW_H
#include <vector>

typedef void (*ARcallback)(float);
class GLFWwindow;
class Window {
    friend void processInput(GLFWwindow *window);
    friend void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    friend class WindowManager;
    static Window *currentWindow;
    static int primaryScreenWidth, primaryScreenHeight;

    GLFWwindow *m_window = nullptr;
    int m_WindowedWidth = 100, m_WindowedHeight = 100;
    int m_xPos = 0, m_yPos = 0;
    bool m_fullScreen = false;
    void resize(int width, int height);

  public:
    void toggleFullscreen();
    Window(int w, int h, const char *name, bool _fullScreen = false);
    void refresh();
    void setTitle(const char *title);
    float getAspectRatio();
    ARcallback *m_ARcallback;
    bool active();
    ~Window();
};

#endif // WINDOW_H
