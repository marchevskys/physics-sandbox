#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <entityx/entityx.h>
#include <glm/glm.hpp>
#include <memory>

namespace ex = entityx;
class Camera;
class PhysWorld;
class Mesh;
class Material;
class Game : public ex::EntityX {
    enum class ObjectType {
        Sphere,
        Cube,
        Icosahedron
    };
    PhysWorld *m_physWorld;
    const Mesh *m_mesh;
    const Material *m_material;

  public:
    explicit Game();

    void update(double dt);

    void render(Camera &cam);

    ex::Entity addObject();

    void addModelToEntity(ex::Entity e);

    glm::dvec3 getPos(ex::Entity e);

    void destroyAllPhysComponents();
    ~Game();
    ;
};

#endif // GAMEAPP_H
