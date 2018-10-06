#include "gameapp.h"
#include "gameobject.h"
#include "logger.h"
#include "mesh.h"

#include "shader.h"
#include "window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/rotate_vector.hpp>
#include <string>

GameApp::GameApp() {
    GameObject::createPhysicsWorld();
    m_window.reset(new Window(800, 600, "Main window" /*, true*/));
}

void GameApp::play() {

    MeshData md;
    //md.generateGeosphere(MeshType::V);
    md.generateCube({1, .1, 1}, MeshType::V);
    Mesh mesh(md);

    GameObject go1(1);
    GameObject go3(3);
    GameObject go2(2);
    //    PhysBody groundPhysics(CollisionCuboid(100, 100, .1));
    //    PhysBody::origin = {0, 0, 2.0};
    //    PhysBody body(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});

    //GameObject obj1(1);

    while (m_window && m_window->active()) {
        GameObject::updateAllPhysics(1.0 / 60.0); // physics update may work async with rendering

        glm::mat4 view(glm::lookAt(glm::vec3(4.0f, 6.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        Shader::setMatricesForAllShaders(1, &view[0][0]);

        glm::mat4 projection(glm::perspective(glm::radians(65.0f), m_window->getAspectRatio(), 0.1f, 10000.0f));
        Shader::setMatricesForAllShaders(2, &projection[0][0]);
        go2.render();
        go3.render();
        go1.render();
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    GameObject::destroyPhysicsWorld();
}
