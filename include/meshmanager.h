#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <vector>
class Mesh;
class MeshManager {
    std::vector<Mesh> primitives;
    MeshManager();
    ~MeshManager();

  public:
    static MeshManager *get() {
        static MeshManager inst;
        return &inst;
    }
    const Mesh *Sphere();
};
#endif // MESHMANAGER_H
