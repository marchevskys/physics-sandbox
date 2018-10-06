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
    MeshData md;
    switch (tempValue) {
    case 1: // sphere
    {
        PhysBody::origin = {0.0, 0.0, 6.0};
        m_body = new PhysBody(CollisionSphere(1.0), {1.0, 1.0, 1.0, 1.0});
        m_body->setForce({0, -3, 0});
        md.generateGeosphere(MeshType::V);
        break;
    }
    case 2: // box
    {
        PhysBody::origin = {0.0, 0.0, 0.0};
        m_body = new PhysBody((CollisionCuboid(100, 100, .1)));
        md.generateCube({100, 100, .1}, MeshType::V);
        break;
    }
    case 3: // box
    {
        PhysBody::origin = {0.0, -50.0, 0.0};
        m_body = new PhysBody((CollisionCuboid(100, .1, 100)));
        md.generateCube({100, .1, 100}, MeshType::V);
        break;
    }
    default: {
        DLOGN("Bad GameObject type")
        throw("");
    }
    }
    m_mesh = new Mesh(md);
    m_shader = new Shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");
    m_shader->use();
    //m_shader = new Shader((ShaderPath + "vertex.glsl").c_str(), (ShaderPath + "fragment.glsl").c_str());
}

void GameObject::render() {
    m_body->getMatrix(m_matrix); // get matrix from physics world
    //DLOGN(m_matrix[0], m_matrix[1], m_matrix[2], m_matrix[3], m_matrix[4], m_matrix[5], m_matrix[6]);
    m_shader->use();                // switch to current shader
    m_shader->setMat4(0, m_matrix); // set matrix to shader

    m_mesh->draw(); // draw mesh with current shader
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
