#ifndef SCENE_H
#define SCENE_H

// internal data, like vertex positions is float by default
// external data, like object positions are scalar (double by default)
typedef double scalar;
typedef unsigned int uint;

enum class MeshType {
    V,
    VN,
    VT,
    VTN,
    VTNB // vertex, texture, normal, binormal
};

class Scene;
Scene *createScene();
void destroyScene(Scene *vScene);
void render(const Scene *vScene);

class Mesh;
Mesh *createMesh(const Scene *vScene, const uint arrayLength, const float *arrayData, const uint indexLength, const uint *indexData, const MeshType type);
//Mesh* createMesh(Mesh* other);
void destroyMesh(Mesh *mesh);
void setMeshMatrix(const Mesh *mesh, scalar *matrix);

class Material;
//Material* createMaterial();
void destroyMaterial(Material *material);
void setMaterial(const Mesh *mesh, const Material *material);

class Camera;
Camera *createCamera(const Scene *vScene);
void destroyCamera(Camera *camera);
void setCameraMatrix(const Camera *camera, scalar *matrix);
#endif // SCENE_H
