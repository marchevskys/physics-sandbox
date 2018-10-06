#ifndef RENDERER_H
#define RENDERER_H

#include <list>
#include <vector>

enum class MeshType {
    V,
    VN,
    VT,
    VTN,
    VTNB // vertex, texture, normal, binormal
};

class Shader;
class Camera;
class Mesh;
class MeshLOD;
class MeshData;
class BBox;

class VisualScene {
    Shader *createShader(const char *const vertexPath, const char *const fragmentPath, const char *const geometryPath);
    void createDefaultShaders();
    void renderMesh(Mesh *) const;

  public:
    VisualScene();

    void renderAll() const;
    void clear();

    //Shader *createShader(const char *const vertexPath, const char *const fragmentPath, const char *const geometryPath);
    //void deleteShader(Shader *shader);

    Mesh *createMesh(const std::vector<float> &pointData, const std::vector<unsigned int> &indexData, MeshType type);
    Mesh *createMesh(MeshData &meshdata);
    void attachMeshMatrix(Mesh *mesh, double *&matrix);
    void deleteMesh(Mesh *mesh);

    Camera *createCamera;
    void attachCameraMatrix(Camera *mesh, double *&matrix);
    void deleteCamera(Camera *camera);

    size_t deleteAllCameras();
    size_t deleteAllShaders();
    size_t deleteAllMeshes();

  private:
    std::vector<Mesh *> meshes;
    //std::vector<Shader> shaders;
};

#endif // RENDERER_H
