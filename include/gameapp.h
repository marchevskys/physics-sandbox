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
    ex::Entity addObject(ObjectType type, const glm::dvec3 &position, bool collision = true);
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
    std::unique_ptr<Material> m_material;
    std::unique_ptr<Material> m_materialWhite;
    ;
};

#endif // GAMEAPP_H
