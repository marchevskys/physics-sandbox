#include "gameobject.h"
#include "logger.h"
#include "mesh.h"
#include "physbody.h"
#include "shader.h"

#include <string>
#include <vector>

std::string ShaderPath("../GameTest/src/shaders/");

GameObject::GameObject(int tempValue) {

    // please don`t kill me :)
    switch (tempValue) {
    case 1: // sphere
        m_body = new PhysBody(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});
        //m_mesh = new Mesh(MeshData::generateSphere(64));
        break;
    case 2: // box
        m_body = new PhysBody((CollisionCuboid(100, 100, .1)));
        //m_mesh = new Mesh(MeshData::generateCube({100, 100, .1}));
        break;
    default:
        throw(std::string(__FUNCTION__) + "Bad GameObject type");
    }

    m_shader = new Shader((ShaderPath + "vertex.glsl").c_str(), (ShaderPath + "pbr.glsl").c_str());
}

void GameObject::render() {
    m_body->getMatrix(m_matrix); // get matrix from physics world

    // if current shader is not previous:
    m_shader->use();                                        // switch to current shader
    m_shader->setMat4("NAME OF VARIABLE", m_matrix.data()); // set matrix to shader
    m_mesh->draw();                                         // draw mesh with current shader
}

GameObject::~GameObject() {
    if (m_body)
        delete m_body;
    if (m_mesh)
        delete m_mesh;
    if (m_shader)
        delete m_shader;
}

void GameObject::updateAllPhysics(double dt = 1.0 / 60.0) {
    PhysBody::updatePhysics(dt);
}

void GameObject::createPhysicsWorld() {
    PhysBody::createWorld();
}

void GameObject::destroyPhysicsWorld() {
    PhysBody::destroyWorld();
}
