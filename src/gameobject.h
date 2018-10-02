#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <array>
#include <vector>

class PhysBody;
class Mesh;
class Shader;

class GameObject {
    std::array<double, 16> m_matrix;
    std::vector<GameObject> m_child;
    PhysBody *m_body = nullptr;
    Mesh *m_mesh = nullptr;
    Shader *m_shader = nullptr;

  public:
    GameObject(int);
    static void updateAllPhysics(double dt);
    static void createPhysicsWorld();
    static void destroyPhysicsWorld();
    void render();
    ~GameObject();
};

#endif // GAMEOBJECT_H
