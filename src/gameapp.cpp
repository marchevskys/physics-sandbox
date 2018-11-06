#include "gameapp.h"

#include "logger.h"

#include "gameobject.h"

#include "physbody.h"
#include "visualscene.h"

#include "window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/rotate_vector.hpp>
#include <list>
#include <string>
#include <vector>

GameApp::GameApp() {
    Physics::PhysBody::createWorld();
    m_window.reset(new Window(800, 600, "Main window", true));
}

void GameApp::play() {

    glm::dmat4 view;

    Physics::PhysBody::origin = {0, 0, -1.5};
    Physics::PhysBody floor(Physics::CollisionCuboid(100, 100, 1));
    Physics::PhysBody::origin = {0, 0, 0.0};

    Graphics::Scene scene;
    Graphics::MeshData mdata;
    mdata.addSphere(80);
    Graphics::Mesh mesh(mdata);
    Graphics::Material material;

    GameObject o1(&scene, &mesh);
    Physics::PhysBody::origin = {0, 1, 2.0};
    GameObject o2(&scene, &mesh);
    Graphics::Camera camera(&scene);
    camera.setFOV(65);

    //o1.getPhysBody()->setForce({-1, -1, 0});

    double constexpr dt = 1.0 / 60.0;
    while (m_window && m_window->active()) {
        Physics::PhysBody::updatePhysics(dt); // physics update may work async with rendering
        scene.clear();

        static double iter = 0;
        iter += .04;

        glm::dvec3 pos, vel;
        o1.getPhysBody()->getPosition(glm::value_ptr(pos));
        o1.getPhysBody()->getVelocity(glm::value_ptr(vel));

        view = glm::lookAt(glm::dvec3(0.01, 0.01, 3.0) + pos, glm::dvec3(0) + pos, glm::dvec3(0.0, 0.0, 1.0));
        //modelMat = glm::rotate(glm::dmat4(1), iter, glm::dvec3(0, 0, 1));

        camera.setMatrix(glm::value_ptr(view));
        camera.setAspectRatio(m_window->getAspectRatio());
        o1.refresh(dt);
        o2.refresh(dt);

        scene.render(camera);
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    Physics::PhysBody::destroyWorld();
}
