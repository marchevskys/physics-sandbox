#ifndef MESHDATA_H
#define MESHDATA_H
#include <array>
#include <vector>

#define DEFAULT_MESH_TYPE Type::V
typedef unsigned int uint;
typedef float Point;
typedef float Normal;
typedef float TexCoord;

class MeshData {
    friend class Mesh;

  public:
    enum class Type {
        UNDEF,
        V,
        VN,
        VT,
        VTN,
        VTNB // vertex, texture, normal, binormal
    };

    //void addCustom(std::vector<float> &&vertexData, std::vector<uint> &&m_indexData, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addCube(float scale_x = 1.f, float scale_y = 1.f, float scale_z = 1.f, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addSphere(uint resolution = 4, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addCapsule(uint resolution = 4, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addIcosahedron(MeshData::Type type = DEFAULT_MESH_TYPE);
    void readFromObj(MeshData::Type type = DEFAULT_MESH_TYPE);

    static std::vector<float> makeSingleArray(const std::vector<float> &positions, const std::vector<float> &normals,
                                              const std::vector<float> &texcoords, const MeshData::Type type);

    //private:
    std::vector<float> m_vertexArray;
    std::vector<uint> m_indexArray;
    MeshData::Type m_type = DEFAULT_MESH_TYPE;
};

#endif // MESHDATA_H
