#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <array>
#include <vector>

class PhysBody;
class Mesh;

class GameObject {
    std::array<double, 16> m_matrix;
    std::vector<GameObject> m_child;
    PhysBody *m_body = nullptr;
    Mesh *m_mesh = nullptr;

  public:
    GameObject();
    GameObject(PhysBody &&pb, Mesh &&m_mesh);
    void update();
    ~GameObject();
};

#endif // GAMEOBJECT_H
