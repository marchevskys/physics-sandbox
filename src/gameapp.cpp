#include "gameapp.h"
#include "gameobject.h"
#include "logger.h"
#include "mesh.h"
#include "shader.h"
#include "window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
extern std::string ShaderPath;
GameApp::GameApp() {
    DLOGN(ShaderPath);
    //GameObject::createPhysicsWorld();
    m_window.reset(new Window(800, 600, "fucking window", true));
}

void GameApp::play() {
    Shader shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");

    MeshData md;
    md.generateGeosphere(MeshType::V);
    Mesh mesh(md);

    //    PhysBody groundPhysics(CollisionCuboid(100, 100, .1));
    //    PhysBody::origin = {0, 0, 2.0};
    //    PhysBody body(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});

    //GameObject obj1(1);

    while (m_window && m_window->active()) {
        static float angle1 = 0;
        static float angle2 = 0;
        angle1 += .02f;
        angle2 += .0033f;
        glm::mat4 view(glm::lookAt(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        glm::mat4 projection(glm::perspective(glm::radians(65.0f), m_window->getAspectRatio(), 0.1f, 100.0f));
        glm::mat4 model(1);
        model = glm::rotate(model, angle2, glm::vec3(0, 0, 1));
        model = glm::rotate(model, angle1, glm::vec3(0, 1, 0));
        shader.use();
        shader.setMat4(0, model);
        shader.setMat4(1, view);
        shader.setMat4(2, projection);
        mesh.draw();
        //GameObject::updateAllPhysics(1.0 / 60.0); // physics update may work async with rendering
        //obj1.render();
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    //GameObject::destroyPhysicsWorld();
}
