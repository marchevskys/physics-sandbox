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
    serializedMeshes.insert({MeshType::Sphere, new Mesh(MeshPrimitives::icosahedron(MeshData::Type::V))});
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
    if (true) {
        //ex.assign<PhysBody>(*m_physWorld, CollisionSphere(*m_physWorld, 1.0), 1.0, 0.1, 0.1, 0.1);
        ex.assign<PhysBody>(m_world, CollisionIcosahedron(m_world, 1.0), 1.0, 0.6, 0.6, 0.6);
        ex.component<PhysBody>()->setPos(&pos[0]);
    }
}

entityx::Entity GameObjectFactory::createCube(glm::dvec3 dimensions, glm::dvec3 pos) {
}

entityx::Entity GameObjectFactory::createIcosahedron(double radius, glm::dvec3 pos) {
}

entityx::Entity GameObjectFactory::createVehicle(const char *const path, glm::dvec3 pos) {
}
