// all visualisations is a problem of VisualScene it visualizes all on "renderAll()" function call
// you task is to mush meshes and cameras inside, delete them if necessary
// you can create multiple visual scenes, they will contain

#ifndef VISUALSCENE_H
#define VISUALSCENE_H
#include "meshdata.h"

#include <memory>
#include <vector>

typedef double scalar;
typedef unsigned int uint;

class Scene;
class Camera;
class MeshData;
class Mesh;
class Material;
class BBox;
class Shader;
class VisualModel;

enum class MeshType {
    V,
    VN,
    VT,
    VTN,
    VTNB // vertex, texture, normal, binormal
};

// mesh and camera container
class Scene {
    friend class VisualModel;
    const std::vector<Shader> &allShaders;
    std::vector<VisualModel *> m_visualmodels;
    size_t numberOfDeletions = 0;
    void addVisualModel(VisualModel *visualmodel);
    void removeMesh(VisualModel *visualmodel);

  public:
    Scene();
    void render(Camera &cam) const;
    void clear() const;
    void clearDepth() const;
    ~Scene();
};

class Camera {
    friend class Scene;
    Scene *m_scene = nullptr;
    scalar *m_matrix = nullptr;

    float m_AspectRatio = 1.f;
    float m_FieldOfView = 65.f;
    float m_NearClip = 0.1f;
    float m_FarClip = 10000.0f;

  public:
    void attachMatrix(double *_mat);
    float getAspectRatio() { return m_AspectRatio; }
    Camera(Scene *scene);
    void setAspectRatio(float _ar) { m_AspectRatio = _ar; }
    void setMatrix(scalar *matrix);
    void setFOV(float fov);
    ~Camera();
};

class VisualModel {
    friend class Scene;
    const Scene *m_scene = nullptr;
    const Material *m_material;
    const Mesh *m_mesh;
    scalar *const m_matrix /*[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}*/;

  public:
    VisualModel(Scene *scene, const Mesh *mesh, const Material *material, scalar *const matrix);
    VisualModel(const VisualModel &other);
    VisualModel(VisualModel &&other);
    ~VisualModel();
};

class Mesh {
    friend class Scene;
    uint m_VAO = 0, m_VBO = 0, m_EBO = 0, m_IndexCount = 0;
    uint m_numVertices = 0, m_arraySize = 0;
    void render() const;

  public:
    Mesh(const MeshData &data);
    //Mesh(MeshData&& data) noexcept;

    Mesh(const Mesh &rhc) = delete;
    Mesh(Mesh &&rhc) = delete;
    uint dataSize() { return m_IndexCount * sizeof(uint) + m_arraySize * sizeof(float); }
    //uint vertexCount() { return m_numVertices; }
    uint triangleCount() { return m_IndexCount / 3; }
    ~Mesh();
};

struct Color {
    float r, g, b;

  public:
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_g){};
    Color(float _val) : r(_val), g(_val), b(_val){};
    const float *data() const { return &r; }
    const float &operator[](int i) const { return *(&r + i); }
};

class Material {
    friend class Scene;
    const Shader *shader = nullptr;
    Color color = {0.5f, 0.5f, 0.5f};
    float metallic = 0.5f;
    float roughness = 0.5f;

  public:
    Material();
    Material(Color _color, float _metallic, float _roughness);
};

class LightSource {
  public:
    scalar *dir;
    Color color;
};

#endif // VISUALSCENE_H
