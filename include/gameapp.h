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
class GLFWwindow;

class Game : public ex::EntityX {

  public:
    enum class ObjectType {
        Sphere,
        Cube,
        Icosahedron
    };
    explicit Game();
    ex::Entity addObject(ObjectType type);
    void addModelToEntity(ex::Entity e);
    glm::dvec3 getPos(ex::Entity e);

    void update(double dt);
    void render(Camera &cam);

    void attachControl(ex::Entity e);
    void destroyAllPhysComponents();
    ~Game();

  private:
    PhysWorld *m_physWorld;
    const Mesh *m_meshSphere;
    const Material *m_material;
    ;
};

#endif // GAMEAPP_H
