#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "logger.h"
#include "mesh.h"
#include "model.h"
#include "physbody.h"
#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <memory>
using namespace std;

int main() {
    try {
        Window window(800, 600, "Main window");

        Game game;
        auto e = game.addObject();
        Camera cam(glm::dvec3(2.0, 2.0, 2.0), glm::dvec3(0.0, 0.0, 0.0));

        DLOG("MAIN LOOP");
        constexpr double dt = 1.0 / 60.0;
        int i = 0;
        glm::dvec3 prevPos = glm::dvec3(0);
        while (window.active()) {
            game.update(dt);
            cam.setAspectRatio(window.getAspectRatio());
            float len = glm::length(game.getPos(e) - glm::dvec3(2.0, 2.0, 2.0));

            //window.setTitle(std::to_string(len).c_str());
            prevPos = game.getPos(e);
            cam.set(glm::dvec3(2.0, 2.0, 2.0), game.getPos(e));
            game.render(cam);
            //            if (i++ == 50)
            //                game.destroyAllPhysComponents();
            window.refresh();
        }
    } catch (const std::exception &except) {
        std::cerr << except.what() << std::endl;
    } catch (...) {
        std::cerr << "Undefined shit happened" << std::endl;
    }
    return 0;
}
