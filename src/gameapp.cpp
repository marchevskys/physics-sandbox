#include "gameapp.h"

#include "logger.h"
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
    //PhysBody::createWorld();
    m_window.reset(new Window(800, 600, "Main window" /*, true*/));
}

void GameApp::play() {
    //VScene scene;
    glm::dmat4 view(glm::lookAt(glm::dvec3(2.0, 3.0, 3.0), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0)));

    // all about mesh
    //PhysBody::origin = {0, 0, -1.0};
    //PhysBody floor(CollisionCuboid(1000, 1000, 1));

    Scene scene;
    MeshData mdata;
    mdata.addIcosahedron();
    Mesh mesh(mdata);
    Material material;

    glm::dmat4 modelMat(1);
    VisualModel model(&scene, &mesh, &material, glm::value_ptr(modelMat));
    Camera camera(&scene);

    while (m_window && m_window->active()) {
        //PhysBody::updatePhysics(1.0 / 60.0); // physics update may work async with rendering
        scene.clear();

        static double iter = 0;
        iter += .04;
        view = glm::lookAt(glm::dvec3(2.0, 3.0, 3.0) * (0.5), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0));
        modelMat = glm::rotate(glm::dmat4(1), iter, glm::dvec3(0, 0, 1));
        camera.setMatrix(glm::value_ptr(view));
        camera.setAspectRatio(m_window->getAspectRatio());
        camera.setFOV(65);
        scene.render(camera);
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    //PhysBody::destroyWorld();
}
