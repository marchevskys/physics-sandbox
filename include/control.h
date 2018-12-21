#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <functional>
#include <map>

using KeyCode = int;
class GLFWwindow;

struct Coordinates {
    double x = 0, y = 0;
};

class Control {
    static bool keys[1024];
    static double scrollOffsetValue;
    static Coordinates mouse;

  public:
    enum class Button { // clang-format off
        Up, Down, Left, Right, Space, Enter,
        W, A, S, D, E, Q
    }; // clang-format on
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    static KeyCode getKey(const char *input);
    static void attachKey(KeyCode k, std::function<void()> f);
    //static void controlPhysBody();
    static double scrollOffset();
    static Coordinates mousePos();
    static bool pressed(Button button);
    Control(){};
};

#endif // CONTROLLER_H
