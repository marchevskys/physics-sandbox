#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "controller.h"
#include "logger.h"
#include "mesh.h"
#include "physbody.h"

#include "model.h"
#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <functional>
#include <glm/gtx/string_cast.hpp>
#include <tuple>

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

    void receive(const ex::ComponentRemovedEvent<Transform> &event) { // If delete transform - delete model and PhysBody
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
        });
    }
};

struct ControlSystem : public ex::System<ControlSystem> {
    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
        es.each<Controller, PhysBody>([dt](ex::Entity entity, Controller &control, PhysBody &body) {

        });
    }
};

Game::Game() {
    systems.add<UpdateTransformSystem>();
    systems.add<RenderSystem>();
    systems.add<ControlSystem>();
    systems.configure();
    m_meshSphere = MeshManager::get()->Sphere();
    m_material = new Material();
    m_physWorld = new PhysWorld();
}

void Game::update(double dt) {
    m_physWorld->update(dt);
    systems.update<UpdateTransformSystem>(dt);
    systems.update<ControlSystem>(dt);
}

void Game::render(Camera &cam) {
    cam.activateShaders();
    systems.update<RenderSystem>(0.0);
}

ex::Entity Game::addObject(ObjectType type) {
    auto ex = entities.create();
    ex.assign<Transform>();
    switch (type) {
    case ObjectType::Sphere: {
        ex.assign<Model>(m_meshSphere, m_material);
        ex.assign<PhysBody>(*m_physWorld, CollisionSphere(*m_physWorld, 1.0), 1.0, 0.1, 0.1, 0.1);
    } break;
    case ObjectType::Cube: {
        PhysBody::setOrigin(0.0, 0.0, -1.0);
        ex.assign<PhysBody>(*m_physWorld, CollisionCuboid(*m_physWorld, 1000, 1000, 1.0), 0.0, 0.0, 0.0, 0.0);
    } break;
    }
    return ex;
}

void Game::addModelToEntity(ex::Entity e) {
    auto modelHandler = e.component<Model>();
    if (modelHandler) {
        const auto &model = modelHandler.get();
    }
}

void Game::attachControl(ex::Entity e) {

    auto bodyHandler = e.component<PhysBody>();
    if (bodyHandler) {
        const auto body = bodyHandler.get();

        KeyCode upKey = Controller::get()->getKey("up");
        KeyCode downKey = Controller::get()->getKey("down");
        KeyCode leftKey = Controller::get()->getKey("left");
        KeyCode rightKey = Controller::get()->getKey("right");
        static double f = 20;

        auto no = [&](PhysBody *body) { body->setTorque(0, 0, 0); };
        auto up = [&](PhysBody *body) { body->setTorque(f, 0, 0); };
        auto down = [&](PhysBody *body) { body->setTorque(-f, 0, 0); };
        auto left = [&](PhysBody *body) { body->setTorque(0, f, 0); };
        auto right = [&](PhysBody *body) { body->setTorque(0, -f, 0); };
        e.assign<Controller>();
        Controller::get()->attachKey(0, std::bind(no, body));
        Controller::get()->attachKey(downKey, std::bind(down, body));
        Controller::get()->attachKey(upKey, std::bind(up, body));
        Controller::get()->attachKey(leftKey, std::bind(left, body));
        Controller::get()->attachKey(rightKey, std::bind(right, body));
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
