#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <array>
#include <vector>

class PhysBody;
class Mesh;

class GameObject
{
    std::array<double, 16> matrix;
    std::vector<GameObject> child;
    PhysBody* body = nullptr;
    Mesh* mesh = nullptr;
public:
    GameObject();
    void update();
    ~GameObject();
};

#endif // GAMEOBJECT_H
