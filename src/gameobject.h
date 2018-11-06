#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <array>
class VisualModel;
class PhysBody;
class Material;
class Scene;
class Mesh;

class GameObject {
    std::unique_ptr<VisualModel> m_model;
    std::unique_ptr<PhysBody> m_body;
    std::unique_ptr<Material> m_material;

    double m_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    double m_force[3] = {0, 0, 0};

  public:
    GameObject(Scene *scene, Mesh *mesh);
    void refresh(double dt);
    PhysBody* getPhysBody() {return m_body.get();};
    VisualModel* getVisualModel() {return m_model.get();};
};
#endif // GAMEOBJECT_H
