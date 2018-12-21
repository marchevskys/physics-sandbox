#include "gameapp.h"
#include "window.h"

#include "camera.h"
#include "control.h"
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
        auto attract = [&es](ex::Entity e1, glm::dvec3 &forceDir, double k) {
            auto comp1 = e1.component<PhysBody>();
            glm::dvec3 pos1;
            comp1.get()->getPos(&pos1[0]);

            es.each<PhysBody>([&](ex::Entity e2, PhysBody &body) {
                auto comp2 = e2.component<PhysBody>();
                if (comp1 != comp2) {
                    glm::dvec3 pos2;
                    comp2.get()->getPos(&pos2[0]);
                    auto dist = glm::length(pos1 - pos2);
                    glm::dvec3 force = k * 4000.0 * (pos1 - pos2) / (dist * dist * dist);
                    forceDir -= force;
                    comp2.get()->addForce(&force[0]);
                }
            });
        };

        es.each<Control, PhysBody>([dt, attract](ex::Entity entity, Control &control, PhysBody &body) {
            glm::dvec3 forceDir(0, 0, 0);
            if (Control::pressed(Control::Button::Up))
                forceDir += glm::dvec3(0, -1, 0);
            if (Control::pressed(Control::Button::Down))
                forceDir += glm::dvec3(0, 1, 0);
            if (Control::pressed(Control::Button::Left))
                forceDir += glm::dvec3(1, 0, 0);
            if (Control::pressed(Control::Button::Right))
                forceDir += glm::dvec3(-1, 0, 0);
            auto forceDirLength = glm::length(forceDir);
            if (forceDirLength > 1.0)
                forceDir /= forceDirLength;
            forceDir *= 300000;
            attract(entity, forceDir, Control::pressed(Control::Button::Space));
            body.addForce(&forceDir[0]);
        });
    }
};

struct AttractSystem : public ex::System<AttractSystem> {
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
        ex.assign<PhysBody>(*m_physWorld, CollisionCuboid(*m_physWorld, 100000, 100000, 1.0), 0.0, 0.0, 0.0, 0.0);
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
        e.assign<Control>();

        glm::dvec4 massMatrix(10000, 5000, 5000, 5000);
        bodyHandler.get()->setMass(&massMatrix[0]);
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
