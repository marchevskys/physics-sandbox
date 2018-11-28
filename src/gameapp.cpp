#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "logger.h"
#include "mesh.h"
#include "model.h"
#include "physbody.h"
#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <memory>
namespace ex = entityx;

struct ComponentEventReceiver : public ex::Receiver<ComponentEventReceiver> {

    ComponentEventReceiver() {}

    void receive(const ex::ComponentAddedEvent<Model> &event) {
        auto e = event.entity;
        Model *model = e.component<Model>().get();
        if (!e.has_component<Transform>())
            THROW("You can't add model to entity without transform");
        if (!model->m_material) {
        }
    }
    // If delete transform - delete model and PhysBody
    void receive(const ex::ComponentRemovedEvent<Transform> &event) {
        auto e = event.entity;
        if (e.has_component<Model>())
            e.component<Model>().remove();
        if (e.has_component<PhysBody>())
            e.component<PhysBody>().remove();
    }
};

struct UpdateTransformSystem : public ex::System<UpdateTransformSystem> {
    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
        es.each<Transform, PhysBody>([dt](ex::Entity entity, Transform &transform, PhysBody &body) {
            body.getMatrix(glm::value_ptr(transform.get()));
        });
    }
};

struct RenderSystem : public ex::System<RenderSystem> {
    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
        es.each<Transform, Model>([dt](ex::Entity entity, Transform &transform, Model &model) {
            model.render(glm::value_ptr(transform.get()));
            //std::string t = glm::to_string(transform.get());
            //THROW(t);
        });
    }
};
class GameData {
  public:
    GameData() {
    }
    ~GameData() {}
};

Game::Game() {
    systems.add<UpdateTransformSystem>();
    systems.add<RenderSystem>();
    systems.configure();
    m_mesh = MeshManager::get()->Sphere();
    m_material = new Material();
    m_physWorld = new PhysWorld();
}

void Game::update(double dt) {
    m_physWorld->update(dt);
    systems.update<UpdateTransformSystem>(dt);
}

void Game::render(Camera &cam) {
    setViewMatricesForAllShaders(&cam.getView()[0][0]);
    setProjectionMatricesForAllShaders(&cam.getProjection()[0][0]);
    systems.update<RenderSystem>(0.0);
}

ex::Entity Game::addObject() {
    auto ex = entities.create();
    ex.assign<Transform>();
    ex.assign<Model>(m_mesh, m_material);
    ex.assign<PhysBody>(*m_physWorld, CollisionSphere(*m_physWorld, 1.0), 1.0, 0.3, 0.3, 0.3);
    return ex;
}

void Game::addModelToEntity(ex::Entity e) {
    if (auto modelHandler = e.component<Model>()) {
        const auto &model = modelHandler.get();
    }
}

glm::dvec3 Game::getPos(ex::Entity e) {
    return e.component<Transform>().get()->getPos();
}

void Game::destroyAllPhysComponents() {
    ex::ComponentHandle<PhysBody> body;
    for (ex::Entity e : entities.entities_with_components(body)) {
        e.component<PhysBody>().remove();
    }
}

Game::~Game() {
    destroyAllPhysComponents(); // destroy all phys bodies before world destruction
    delete m_physWorld;
    delete m_material;
}
