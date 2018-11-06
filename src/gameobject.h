#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <array>
#include <memory>

#include "declarations.h"

class GameObject {
    std::unique_ptr<Graphics::VisualModel> m_model;
    std::unique_ptr<Graphics::Material> m_material;
    std::unique_ptr<Physics::PhysBody> m_body;

    double m_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    double m_force[3] = {0, 0, 0};

  public:
    GameObject(Graphics::Scene *scene, Graphics::Mesh *mesh);
    void refresh(double dt);
    Physics::PhysBody *getPhysBody() { return m_body.get(); };
    Graphics::VisualModel *getVisualModel() { return m_model.get(); };
};
#endif // GAMEOBJECT_H
