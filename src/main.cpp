#include "gameapp.h"
#include "logger.h"

#include "window.h"

#include <exception>
#include <iostream>
#include <string>
using namespace std;

int main() {
    try {

        GameApp app;
        app.play();

    } catch (const char *except) {
        std::cerr << except << std::endl;
    } catch (const string except) {
        std::cerr << except << std::endl;
    } catch (...) {
        std::cerr << "Undefined shit happens" << std::endl;
    }
}
