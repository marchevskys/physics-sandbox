#include "gameobjectfactory.h"

#include "logger.h"
#include "mesh.h"
#include "meshdata.h"
#include "model.h"
#include "physbody.h"

#include "transform.h"

#include <entityx/entityx.h>
std::map<GameObjectFactory::MeshType, Mesh *> GameObjectFactory::serializedMeshes;

GameObjectFactory::GameObjectFactory(entityx::EntityX &game, const PhysWorld &world) : m_game(game), m_world(world) {
    serializedMeshes.insert({MeshType::Sphere, new Mesh(MeshPrimitives::sphere(10))});
    serializedMeshes.insert({MeshType::Cube, new Mesh(MeshPrimitives::cube())});
    serializedMeshes.insert({MeshType::Icosahedron, new Mesh(MeshPrimitives::icosahedron())});
}

GameObjectFactory::~GameObjectFactory() {
    for (auto &s : serializedMeshes)
        delete s.second;
}

entityx::Entity GameObjectFactory::createSphere(double radius, glm::dvec3 pos) {
    ex::Entity ex = m_game.entities.create();
    ex.assign<Transform>();
    ex.component<Transform>()->setPos(pos);
    ex.assign<Model>(serializedMeshes.at(MeshType::Sphere));

    ex.assign<PhysBody>(m_world, CollisionSphere(m_world, 1.0), 1.0, vec3d(0.6, 0.6, 0.6));
    //ex.assign<PhysBody>(m_world, CollisionIcosahedron(m_world, 1.0), 1.0, vec3d(0.6, 0.6, 0.6));
    ex.component<PhysBody>()->setPos(pos);
    return ex;
}

entityx::Entity GameObjectFactory::createCube(glm::dvec3 dimensions, glm::dvec3 pos) {
    ex::Entity ex = m_game.entities.create();
    ex.assign<Transform>();
    ex.component<Transform>()->setPos(pos);
    ex.assign<Model>(serializedMeshes.at(MeshType::Cube));

    //glm::dvec3 poss{0.0,0.0, -1.0};
    ex.assign<PhysBody>(m_world, CollisionCuboid(m_world, {100000, 100000, 1.0}), 0.0, vec3d(0.0, 0.0, 0.0));
    ex.component<PhysBody>()->setPos(pos);
    return ex;
}

entityx::Entity GameObjectFactory::createIcosahedron(double radius, glm::dvec3 pos) {
}

entityx::Entity GameObjectFactory::createVehicle(glm::dvec3 pos) {
    ex::Entity ex = m_game.entities.create();
    return ex;
}

entityx::Entity GameObjectFactory::createVehicle(const char *const path, glm::dvec3 pos) {
}
