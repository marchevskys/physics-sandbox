#ifndef OBJECT_H
#define OBJECT_H
#include <vector>

class PhysBody;
class Mesh;

class Component;
class Object {
    Object *m_parent = nullptr;
    PhysBody *m_body = nullptr;
    Mesh *m_mesh;
    double m_matrix[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    std::vector<Object *> children;
    std::vector<Component *> components;

  public:
    Object(Object *_parent = nullptr) : m_parent(_parent) {
    }

    void addChild(Object *){};
    void refreshPhysics() {
        if (m_body)
            for (const auto &c : children)
                c->refreshPhysics();
    }

    void refreshGraphics() {
        // push matrices to VisualScene
    }
};
#endif // OBJECT_H
