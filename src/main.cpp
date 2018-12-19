#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "controller.h"
#include "logger.h"
#include "mesh.h"
#include "model.h"
#include "physbody.h"
#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include <chrono>
#include <memory>

class Timer {
  public:
    typedef std::chrono::high_resolution_clock Clock;
    Timer() { epoch = Clock::now(); }
    double reset() {
        auto diff = Clock::now() - epoch;
        epoch = Clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    }
    Clock::duration time_elapsed() const { return Clock::now() - epoch; }

  private:
    Clock::time_point epoch;
};

using namespace std;

int main() {
    try {
        Window window(800, 600, "Main window");

        Game game;
        PhysBody::setOrigin(0.0, 0.0, 0.0);
        game.addObject(Game::ObjectType::Cube);

        PhysBody::setOrigin(0.0, 0.0, 1.0);
        auto e = game.addObject(Game::ObjectType::Sphere);
        //        auto b = e.component<PhysBody>().get();

        game.attachControl(e);

        for (int i = 0; i < 13; ++i) {
            auto pos = glm::sphericalRand<double>(10.0);
            PhysBody::setOrigin(pos[0], pos[1], 10.0 + pos[2]);
            game.addObject(Game::ObjectType::Sphere);
        }

        Camera cam(glm::dvec3(20.0, 2.0, 20.0), glm::dvec3(0.0, 0.0, 1.0));

        DLOG("MAIN LOOP");
        constexpr double dt = 1.0 / 60.0;
        while (window.active()) {
            game.update(dt);
            cam.setAspectRatio(window.getAspectRatio());

            auto pos = game.getPos(e);
            cam.set(glm::dvec3(2.0, 2.0, 2.0), glm::dvec3(0.0, 0.0, 0.0) + pos);
            game.render(cam);

            window.refresh();
        }
    } catch (const std::exception &except) {
        std::cerr << except.what() << std::endl;
    } catch (...) {
        std::cerr << "Undefined shit happened" << std::endl;
    }
    return 0;
}
