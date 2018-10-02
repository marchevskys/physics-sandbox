#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;
class Window {
    GLFWwindow *m_window = nullptr;
    int m_width = 100, m_height = 100;

  public:
    Window(int w, int h, const char *name, bool _fillScreen = false);
    void refresh();
    //void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    float getAspectRatio();
    void resize(int width, int height);
    int active();
    ~Window();

    static Window *currentWindow;
};

#endif // WINDOW_H
