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
    PhysBody::createWorld();
    m_window.reset(new Window(800, 600, "Main window" /*, true*/));
}

class GameObject;

class MeshCreator {
    MeshCreator(){};
    ~MeshCreator(){};

  public:
};
class GameObjectCreator {
  public:
    static std::unique_ptr<GameObject> createIcosahedron() {
    }
};

class GameObject {
    std::unique_ptr<VisualModel> m_model;
    std::unique_ptr<PhysBody> m_body;
    std::unique_ptr<Material> m_material;
    scalar m_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  public:
    GameObject(Scene *scene, Mesh *mesh) {
        m_material = std::make_unique<Material>();
        m_body.reset(new PhysBody(CollisionSphere(1), {1, 1, 1, 1}));
        m_model.reset(new VisualModel(scene, mesh, m_material.get(), m_matrix));
    }
    void refresh(double dt) { m_body->getMatrix(m_matrix); }
};

void GameApp::play() {

    glm::dmat4 view;

    PhysBody::origin = {0, 0, -1.0};
    PhysBody floor(CollisionCuboid(1000, 1000, 1));
    PhysBody::origin = {0, 0, 10.0};

    Scene scene;
    MeshData mdata;
    mdata.addIcosahedron();
    Mesh mesh(mdata);
    Material material;

    GameObject object(&scene, &mesh);
    Camera camera(&scene);
    camera.setFOV(65);

    while (m_window && m_window->active()) {
        PhysBody::updatePhysics(1.0 / 60.0); // physics update may work async with rendering
        scene.clear();

        static double iter = 0;
        iter += .004;
        view = glm::lookAt(glm::dvec3(2.0, 3.0, 0.0) * (1.4), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0));
        //modelMat = glm::rotate(glm::dmat4(1), iter, glm::dvec3(0, 0, 1));
        view = glm::rotate(view, iter, glm::dvec3(0, 0, 1));
        camera.setMatrix(glm::value_ptr(view));
        camera.setAspectRatio(m_window->getAspectRatio());
        object.refresh(1 / 60);

        scene.render(camera);
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    PhysBody::destroyWorld();
}
