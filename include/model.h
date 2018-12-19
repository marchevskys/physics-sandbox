#ifndef MODEL_H
#define MODEL_H
#include <vector>

class Mesh;
class Shader;

class Material {
  public:
    const Shader *m_shader;
    Material();
    float r = 0.5f, g = 0.5f, b = 0.5f;
};

class Model {
  public:
    Mesh *m_mesh = nullptr;
    Material *m_material = nullptr;
    Model();
    Model(const Mesh *mesh);
    Model(const Mesh *mesh, const Material *material);
    void render(const double *transform);
};

class MeshManager {
    std::vector<Mesh> meshes;
    MeshManager();
    ~MeshManager();

  public:
    static MeshManager *get() {
        static MeshManager inst;
        return &inst;
    }
    const Mesh *Sphere();
};

void setViewMatricesForAllShaders(const double *mat);
void setProjectionMatricesForAllShaders(const float *mat);
void setviewPos(const float *vec);

#endif // MODEL_H
