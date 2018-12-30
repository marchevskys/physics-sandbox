#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include <entityx/Entity.h>
#include <glm/glm.hpp>
#include <map>

// clang-format off
namespace entityx { class EntityX; }
class PhysWorld;
class Texture;
class Mesh;
// clang-format on

namespace ex = entityx;

struct Params {
    glm::vec3 color;
    Texture *texture;
    bool collision;
};

class GameObjectFactory {
  public:
    GameObjectFactory(entityx::EntityX &game, const PhysWorld &world);
    ~GameObjectFactory();
    ex::Entity createSphere(double radius, glm::dvec3 pos);
    ex::Entity createCube(glm::dvec3 dimensions, glm::dvec3 pos);
    ex::Entity createIcosahedron(double radius, glm::dvec3 pos);

    ex::Entity createVehicle(const char *const path, glm::dvec3 pos);

    enum class MeshType { Empty = 0,
                          Sphere,
                          Cube,
                          Icosahedron,
                          Capsule,
                          Piramyd,
                          Custom
    };

  private:
    entityx::EntityX &m_game;
    const PhysWorld &m_world;
    static std::map<MeshType, Mesh *> serializedMeshes;
};

#endif // GAMEOBJECTFACTORY_H
