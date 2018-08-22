#include "gameapp.h"

#include "logger.h"
#include "physbody.h"
GameApp::GameApp() {
    PhysBody::createWorld();
}

void GameApp::play() {

    double massMatrix[4];
    PhysBody ground(CollisionCuboid(100, 100, .1));
    PhysBody::origin = {0, 0, 2.0};
    PhysBody body(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});
    //double force[3]{0, 100, 0};
    //body.setForce(force);

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
