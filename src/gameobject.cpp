#include "gameobject.h"
#include "mesh.h"
#include "physbody.h"
#include "shader.h"

GameObject::GameObject() {
    m_body = new PhysBody(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});
    m_mesh = new Mesh(MeshData::generateCube());
}

void GameObject::update() {
    m_body->getMatrix(m_matrix.data());
    // upload matrix to shader
    m_mesh->draw();
}

GameObject::~GameObject() {
    if (m_body)
        delete m_body;
    if (m_mesh)
        delete m_mesh;
}
