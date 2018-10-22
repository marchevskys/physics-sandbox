// all visualisations is a problem of VisualScene it visualizes all on "renderAll()" function call
// you task is to mush meshes and cameras inside, delete them if necessary
// you can create multiple visual scenes, they will contain

#ifndef RENDERER_H
#define RENDERER_H
#include "meshdata.h"

#include <memory>
#include <vector>

typedef double scalar;

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
    friend class Camera;
    std::vector<VisualModel *> m_visualmodels;
    size_t numberOfDeletions = 0;
    void addVisualModel(VisualModel *visualmodel);
    void removeMesh(VisualModel *visualmodel);

  public:
    Scene();
    void clear();
    ~Scene();
};

class Camera {
    Scene *m_scene = nullptr;
    scalar *m_matrix = nullptr;

    float m_AspectRatio = 1.f;
    float m_FieldOfView = 65.f;
    float m_NearClip = 0.1f;
    float m_FarClip = 10000.0f;

    float getAspectRatio() { return m_AspectRatio; }
    void setAspectRatio(float _ar) { m_AspectRatio = _ar; }
    void attachMatrix(double *_mat);

  public:
    Camera(const Scene &m_scene);
    void setMatrix(scalar *matrix);
    void takeShot();
    ~Camera();
};

class VisualModel {
    Scene *m_scene = nullptr;
    const size_t id;
    static size_t idCounter;
    scalar *m_matrix /*[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}*/;
    struct LOD {
        std::shared_ptr<Material> m_material;
        std::shared_ptr<Mesh> m_mesh;
        scalar m_distance;
    };
    std::vector<LOD> m_LOD;
    void render(const scalar distance);

  public:
    VisualModel(Scene &scene);
    VisualModel(VisualModel &&other);
    VisualModel(const VisualModel &other);
    ~VisualModel();
};

class Mesh {
    unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0, m_VertexCount = 0;
    void render();

  public:
    Mesh(const MeshData &data);
    Mesh(const Mesh &rhc);
    Mesh(Mesh &&rhc);
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
    Shader *shader = nullptr;
    Color color = {0.5f, 0.5f, 0.5f};
    float metallic = 0.5f;
    float roughness = 0.5f;

  public:
    Material();
    Material(Color _color, float _metallic, float _roughness);
};

#endif // RENDERER_H
