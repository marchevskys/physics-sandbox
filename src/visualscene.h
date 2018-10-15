// all visualisations is a problem of VisualScene it visualizes all on "renderAll()" function call
// you task is to mush meshes and cameras inside, delete them if necessary
// you can create multiple visual scenes, they will contain

#ifndef RENDERER_H
#define RENDERER_H
#include "meshdata.h"

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

class VScene {
    void createDefaultShaders();

  public:
    VScene();

    void renderAll() const;
    void clear();
    void clearDepth();

    //Shader *createShader(const char *const vertexPath, const char *const fragmentPath, const char *const geometryPath);
    //void deleteShader(Shader *shader);

    Mesh *createMesh(const MeshData &data, double *matrix);
    void attachMeshMatrix(Mesh *mesh, double *&matrix);
    void deleteMesh(Mesh *mesh);

    Camera *createCamera(double *_matrix, float _fov, float _ar);
    void attachCameraMatrix(Camera *camera, double *&matrix);
    void setCameraAR(float _ar);
    void deleteCamera(Camera *camera);

    size_t deleteAllCameras();
    size_t deleteAllShaders();
    size_t deleteAllMeshes();
    ~VScene();

  private:
    std::vector<Mesh *> meshes;
    Shader *shader;
    Shader *shadowShader;
    Camera *camera;
    //std::vector<Shader> shaders;
};

#endif // RENDERER_H
