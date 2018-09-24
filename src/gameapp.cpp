#include "gameapp.h"

#include "logger.h"
#include "physbody.h"
#include "window.h"

GameApp::GameApp() {
    PhysBody::createWorld();

    m_window.reset(new Window(800, 600, "fucking window"));
}

void GameApp::play() {

    double massMatrix[4];
    PhysBody ground(CollisionCuboid(100, 100, .1));
    PhysBody::origin = {0, 0, 2.0};
    PhysBody body(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});
    //double force[3]{0, 100, 0};
    //body.setForce(force);

    while (m_window->active()) {
        // todo
        m_window->refresh();
    }

    for (int i = 0; i < 200; i++) {
        PhysBody::updatePhysics(1.0 / 60.0);
    }
}

void GameApp::physicsUpdate() {
}

void GameApp::graphicsUpdate() {
}

GameApp::~GameApp() {
    PhysBody::destroyWorld();
}
