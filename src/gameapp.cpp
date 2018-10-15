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

void GameApp::play() {
    VScene scene;
    glm::dmat4 view(glm::lookAt(glm::dvec3(2.0, 3.0, 3.0), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0)));
    scene.createCamera(&view[0][0], 75.0f, m_window->getAspectRatio());

    // all about mesh

    struct Mood {
        float anger = 0.0f;
        float maxLevel = 1.0f;
        bool lovePeople = false;
        Mood() {
            anger = glm::linearRand(0.0f, 1.0f);
            lovePeople = rand() % 2;
        }

        bool loveSociety(bool someoneIsNearby) {
            if (anger < 0)
                lovePeople = true;
            else if (anger > maxLevel)
                lovePeople = false;
            anger = someoneIsNearby ? (anger += 0.01f) : (anger -= 0.01f);
            anger = anger < -maxLevel ? -maxLevel : anger;
            anger = anger > maxLevel * 2 ? maxLevel * 2 : anger;
            return lovePeople;
        }
    };

    PhysBody::origin = {0, 0, -1.0};
    PhysBody floor(CollisionCuboid(1000, 1000, 1));

    MeshData mdata;
    mdata.addGeosphere();
    double identityMatrix[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    scene.createMesh(mdata, identityMatrix);

    class Component;
    class Object {
        Object *m_parent = nullptr;
        PhysBody *m_body = nullptr;
        Mesh *m_mesh;
        double m_matrix[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        std::vector<Object *> children;
        std::vector<Component *> components;

      public:
        Object(Object *_parent = nullptr) : m_parent(_parent) {
        }

        void addChild(Object *){};
        void refreshPhysics() {
            if (m_body)
                for (const auto &c : children)
                    c->refreshPhysics();
        }

        void refreshGraphics() {
            // push matrices to VisualScene
        }
    };

    //    class ObjectCreator
    //    {
    //        ObjectCreator(const VisualScene& scene){};
    //        Object* createGeosphere();
    //        ~ObjectCreator(){};
    //    };

    //    1.an id (whether it be a string or int)
    //    2.a vector of components
    //    3.an entity of a parent, if null, this is a root
    //    4.a vector of entities as children.

    while (m_window && m_window->active()) {
        PhysBody::updatePhysics(1.0 / 60.0); // physics update may work async with rendering
        scene.clear();
        scene.setCameraAR(m_window->getAspectRatio());

        static double iter = 0;
        view = glm::lookAt(glm::dvec3(2.0, 3.0, 3.0) * (1. + (iter += .005)), glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 1.0));

        scene.renderAll();
        m_window->refresh(); // it clears screen as well
    }
}

GameApp::~GameApp() {
    PhysBody::destroyWorld();
}
