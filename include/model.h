#ifndef MODEL_H
#define MODEL_H

class Mesh;
class Shader;

struct Color {
    float r, g, b;
    Color(float _r = 0.5f, float _g = 0.5f, float _b = 0.5f) : r(_r), g(_g), b(_b) {}
};

class Material {
  public:
    const Shader *m_shader;
    Material();
    Color m_color;
};

class Model {
  public:
    Mesh *m_mesh = nullptr;
    Material m_material;
    Model();
    Model(const Mesh *mesh);
    Model(const Mesh *mesh, const Material material);
    void render(const double *transform);
};

#endif // MODEL_H
