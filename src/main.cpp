#include "gameapp.h"
#include <exception>
#include <iostream>
using namespace std;

int main() {
    try {
        GameApp app;
    } catch (const std::exception &except) {
        std::cerr << except.what() << std::endl;
    } catch (...) {
        std::cerr << "Undefined shit happened" << std::endl;
    }
    return 0;
}
