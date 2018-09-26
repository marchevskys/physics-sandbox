#ifndef MESH_H
#define MESH_H

#include<glm/fwd.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

typedef float scalar;
typedef unsigned int uint;

enum MeshType{
    V, VN, VT, VTN, VTNTB   // vertex, texture, normal, tangent, binormal
};
#define DEFAULT_MESH_TYPE VTN

class MeshData
{
    friend class Mesh;
    //uint hash = 0;
    std::vector<scalar> m_vertexArray;
    std::vector<uint> indices;
    MeshType m_type;
  public:
    static MeshData generateCube(MeshType type = DEFAULT_MESH_TYPE);
    static MeshData generateSphere(uint resolution, MeshType type = DEFAULT_MESH_TYPE);
    static MeshData generateCapsule(uint resolution, MeshType type = DEFAULT_MESH_TYPE);
    static MeshData generateGeosphere(MeshType type = DEFAULT_MESH_TYPE);
    static MeshData readFromObj(MeshType type = DEFAULT_MESH_TYPE);
};

class Mesh {
    uint VAO = 0, VBO = 0, EBO = 0;
    uint numOfVertices = 0;
  public:
    Mesh(MeshData&& mData);
    void draw();
    ~Mesh();
};

class LODMesh
{
    std::vector<std::pair<Mesh, scalar>> lods;
  public:
    void draw(scalar distFactor);
    LODMesh();
    ~LODMesh();
};

#endif // MESH_H
