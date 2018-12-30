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
    ;
    explicit Game();
    ex::Entity addSphere(const glm::dvec3 &position, bool collision = true);
    ex::Entity addCube(const glm::dvec3 &position, bool collision = true);
    void addModelToEntity(ex::Entity e);
    glm::dvec3 getPos(ex::Entity e);

    void update(double dt);
    void render(Camera &cam);

    void attachControl(ex::Entity e);
    void destroyAllPhysComponents();
    ~Game();
    const PhysWorld &getPhysWorld() { return *m_physWorld; }

  private:
    PhysWorld *m_physWorld;
    std::unique_ptr<Mesh> m_meshSphere;
    std::unique_ptr<Material> m_material;
};

#endif // GAMEAPP_H
