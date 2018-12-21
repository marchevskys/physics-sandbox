#include "control.h"
#include "logger.h"
#include "string"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>

bool Control::keys[1024]{0};
double Control::scrollOffsetValue = 0.0;
Coordinates Control::mouse{0.0, 0.0};

void Control::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key > 10 && key < 300) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void Control::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    mouse.x = xpos;
    mouse.x = ypos;
}

void Control::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollOffsetValue = yoffset;
}

double Control::scrollOffset() {
    double value = scrollOffsetValue;
    scrollOffsetValue = 0;
    return value;
}

Coordinates Control::mousePos() {
    return mouse;
}

bool Control::pressed(Control::Button button) {
    switch (button) {
    case Control::Button::Up: {
        return keys[GLFW_KEY_UP];
    } break;
    case Control::Button::Down: {
        return keys[GLFW_KEY_DOWN];
    } break;
    case Control::Button::Left: {
        return keys[GLFW_KEY_LEFT];
    } break;
    case Control::Button::Right: {
        return keys[GLFW_KEY_RIGHT];
    } break;
    case Control::Button::Space: {
        return keys[GLFW_KEY_SPACE];
    } break;
    case Control::Button::Enter: {
        return keys[GLFW_KEY_ENTER];
    } break;
    case Control::Button::W: {
        return keys[GLFW_KEY_W];
    } break;
    case Control::Button::A: {
        return keys[GLFW_KEY_A];
    } break;
    case Control::Button::S: {
        return keys[GLFW_KEY_S];
    } break;
    case Control::Button::D: {
        return keys[GLFW_KEY_D];
    } break;
    case Control::Button::E: {
        return keys[GLFW_KEY_E];
    } break;
    case Control::Button::Q: {
        return keys[GLFW_KEY_Q];
    } break;
    default:
        return false;
    }
}
