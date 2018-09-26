#include "gameapp.h"

#include "logger.h"

#include "gameobject.h"
#include "mesh.h"
#include "physbody.h"
#include "window.h"

GameApp::GameApp() {
    PhysBody::createWorld();

    m_window.reset(new Window(800, 600, "fucking window"));
}

void GameApp::play() {

    //    PhysBody groundPhysics(CollisionCuboid(100, 100, .1));
    //    PhysBody::origin = {0, 0, 2.0};
    //    PhysBody body(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});

    Mesh mesh(MeshData::generateCube());
    while (m_window && m_window->active()) {
        // todo
        PhysBody::updatePhysics(1.0 / 60.0);
        m_window->refresh();
    }

    for (int i = 0; i < 200; i++) {
    }
}

void GameApp::physicsUpdate() {
}

void GameApp::graphicsUpdate() {
}

GameApp::~GameApp() {
    PhysBody::destroyWorld();
}
