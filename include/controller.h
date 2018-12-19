#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <functional>
#include <map>

using KeyCode = int;
class GLFWwindow;
class Controller {
    //    int keyUp = 0, keyDown = 0, keyLeft = 0, keyRight = 0;
    static std::map<KeyCode, std::function<void()>> keyBindings;

  public:
    static bool keys[1024];
    static void processInput(GLFWwindow *window, int key, int scancode, int action, int mode);
    static KeyCode getKey(const char *input);
    static void attachKey(KeyCode k, std::function<void()> f);
    static Controller *get() {
        static Controller inst;
        return &inst;
    }
};

#endif // CONTROLLER_H
