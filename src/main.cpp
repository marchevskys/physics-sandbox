#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "control.h"
#include "gameobjectfactory.h"
#include "logger.h"
#include "mesh.h"
#include "model.h"
#include "physbody.h"
#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include <chrono>
#include <memory>
#include <numeric>

using namespace std;

int main() {
    try {
        Window window(1024, 768, "Main window", false);
        Game game;
        GameObjectFactory factory(game, game.getPhysWorld());

        factory.createCube({10000.0, 10000.0, 1.0}, {0.0, 0.0, -1.0});
        factory.createVehicle({0.0, 0.0, 0.0});

        auto e1 = factory.createSphere(1.0, {2.0, 0.0, 4.0});
        auto e2 = factory.createSphere(1.0, {6.0, 0.0, 4.0});
        auto p1 = e1.component<PhysBody>().get();
        auto p2 = e2.component<PhysBody>().get();
        p1->setUserJoint(*p2);
        //auto bodyHandler = e.component<PhysBody>();
        game.attachControl(e1);

        //        for (int i = 0; i < 111; ++i) {
        //            auto pos = glm::ballRand<double>(10.0) + glm::dvec3(0, 0, 10);
        //            game.addSphere(pos, true);
        //        }

        Camera cam(glm::dvec3(20.0, 2.0, 20.0), glm::dvec3(0.0, 0.0, 1.0));
        cam.setFOV(1.4f);
        DLOG("MAIN LOOP");

        std::vector<glm::dvec3> prevCamPositions(14, glm::dvec3(0));
        constexpr double dt = 1.0 / 60.0;

        double xx = M_PI_2 - 0.01, yy = -0.5;
        while (window.active()) {
            game.update(dt);
            cam.setAspectRatio(window.getAspectRatio());

            static int currentPos = 0;
            auto pos = game.getPos(e1);
            prevCamPositions[currentPos++ % prevCamPositions.size()] = pos;
            glm::dvec3 init(0);
            auto finalPos = std::accumulate(prevCamPositions.begin(), prevCamPositions.end(), init) / static_cast<double>(prevCamPositions.size());
            static double distance = 13.0;

            glm::dvec3 vec = {1, 0, 0};
            xx += Control::mousePos().x * 0.01;
            yy += Control::mousePos().y * 0.01;
            yy = yy > 0.0 ? 0.0 : yy;
            yy = yy < -M_PI_2 ? -M_PI_2 + 0.001 : yy;
            vec = glm::rotate(vec, yy, glm::dvec3(0, 1, 0));
            vec = glm::rotate(vec, xx, glm::dvec3(0, 0, 1));

            distance *= 1.0 + Control::scrollOffset() * 0.1;

            distance = glm::clamp<double>(distance, 0.001, 1000);
            cam.set(vec * distance + finalPos, glm::dvec3(0.0, 0.0, 0.0) + finalPos);
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
