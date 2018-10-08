#include "gameapp.h"
#include "gameobject.h"
#include "logger.h"

#include "visualscene.h"

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
    VisualScene scene;
    glm::dmat4 view(glm::lookAt(glm::dvec3(2.0, 3.0, 1.0), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0)));
    scene.createCamera(&view[0][0], 65.f, m_window->getAspectRatio());

    // all about mesh
    double matrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    double matrix2[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    std::vector<uint> indices{
        2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 1, 5, 0, 11, 6, 7, 11, 7,
        8, 11, 8, 9, 11, 9, 10, 11, 10, 6, 1, 2, 6, 2, 3, 7, 3, 4, 8, 4,
        5, 9, 5, 1, 10, 2, 7, 6, 3, 8, 7, 4, 9, 8, 5, 10, 9, 1, 6, 10};

    std::vector<float> positions{
        0.000f, 0.000f, 1.000f,
        0.894f, 0.000f, 0.447f,
        0.276f, 0.851f, 0.447f,
        -0.724f, 0.526f, 0.447f,
        -0.724f, -0.526f, 0.447f,
        0.276f, -0.851f, 0.447f,
        0.724f, 0.526f, -0.447f,
        -0.276f, 0.851f, -0.447f,
        -0.894f, 0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
        0.724f, -0.526f, -0.447f,
        0.000f, 0.000f, -1.000f};

    scene.createMesh(positions, indices, MeshType::V, matrix);
    scene.createMesh(positions, indices, MeshType::V, matrix2);
    while (m_window && m_window->active()) {
        GameObject::updateAllPhysics(1.0 / 60.0); // physics update may work async with rendering
        scene.setCameraAR(m_window->getAspectRatio());
        static double variable = 0;
        variable += 0.05;
        matrix[12] = 2 * sin(variable);
        matrix[13] = 2 * cos(variable);
        matrix[14] = .2 * sin(variable * 7);
        scene.renderAll();

        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    GameObject::destroyPhysicsWorld();
}
