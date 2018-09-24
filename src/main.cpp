#include "gameapp.h"
#include "logger.h"

#include "window.h"

#include <exception>
#include <iostream>
using namespace std;

int main() {
    try {

        GameApp app;
        app.play();

    } catch (const char *except) {
        std::cerr << except << std::endl;
    }
}
