#include "gameobject.h"

#include "physbody.h"
#include "visualscene.h"

GameObject::GameObject(Scene *scene, Mesh *mesh) {

    m_material = std::make_unique<Material>();
    m_body.reset(new PhysBody(CollisionSphere(1), {1, 1, 1, 1}));
    m_model.reset(new VisualModel(scene, mesh, m_material.get(), m_matrix));
}

void GameObject::refresh(double dt) {
    m_body->getMatrix(m_matrix);
}
