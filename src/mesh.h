#ifndef MESH_H
#define MESH_H

#include <glm/fwd.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <string>
#include <vector>

typedef float scalar;
typedef unsigned int uint;

enum MeshType {
    V,
    VN,
    VT,
    VTN,
    VTNB // vertex, texture, normal, binormal
};
#define DEFAULT_MESH_TYPE V

class MeshData {
    friend class Mesh;
    //uint hash = 0;
    std::vector<scalar> m_vertexArray;
    std::vector<uint> m_indexArray;
    MeshType m_type = DEFAULT_MESH_TYPE;

  public:
    void generateCube(std::array<float, 3> scale = {1.0f, 1.0f, 1.0f}, MeshType type = DEFAULT_MESH_TYPE);
    void generateSphere(uint resolution, MeshType type = DEFAULT_MESH_TYPE);
    void generateCapsule(uint resolution, MeshType type = DEFAULT_MESH_TYPE);
    void generateGeosphere(MeshType type = DEFAULT_MESH_TYPE);
    void readFromObj(MeshType type = DEFAULT_MESH_TYPE);
};

class Mesh {
    uint VAO = 0, VBO = 0, EBO = 0;
    uint numOfVertices = 0;

  public:
    Mesh();
    Mesh(const MeshData &mData);
    void draw();
    ~Mesh();
};

#endif // MESH_H
