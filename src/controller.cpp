#include "controller.h"
#include "logger.h"
#include "string"
#include <GLFW/glfw3.h>
#include <algorithm>

bool Controller::keys[1024]{0};
std::map<KeyCode, std::function<void()>> Controller::keyBindings;

void Controller::processInput(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key > 200 && key < 300) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;

        auto it = keyBindings.find(key);
        if (it != keyBindings.end())
            it->second();
        if (!action)
            keyBindings[0]();
        //        for (const auto &k : keyBindings) {
        //            //       DLOGN(k.first, &k.second);
        //            if (keys[k.first])
        //                k.second();
        //            else
        //                keyBindings[0]();
        //        }
        //DLOG("bindings: ", keyBindings.size(), "  current: ", key, action ? " pressed" : "released");
    }
}

KeyCode Controller::getKey(const char *input) {
    std::string str(input);
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "up")
        return GLFW_KEY_UP;
    else if (str == "down")
        return GLFW_KEY_DOWN;
    else if (str == "left")
        return GLFW_KEY_LEFT;
    else if (str == "right")
        return GLFW_KEY_RIGHT;
}

void Controller::attachKey(KeyCode k, std::function<void()> f) {
    keyBindings.insert(std::make_pair(k, f));
}
