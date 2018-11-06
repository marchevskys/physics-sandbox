#include "gameobject.h"

#include "physbody.h"
#include "visualscene.h"

GameObject::GameObject(Graphics::Scene *scene, Graphics::Mesh *mesh) {

    m_material = std::make_unique<Graphics::Material>();
    m_body.reset(new Physics::PhysBody(Physics::CollisionSphere(1), {1, 1, 1, 1}));
    m_model.reset(new Graphics::VisualModel(scene, mesh, m_material.get(), m_matrix));
}

void GameObject::refresh(double dt) {
    m_body->getMatrix(m_matrix);
}
