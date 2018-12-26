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

#endif // MODEL_H