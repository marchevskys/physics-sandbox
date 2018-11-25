#ifndef MESHDATA_H
#define MESHDATA_H
#define DEFAULT_MESH_TYPE Type::V

#include <array>
#include <vector>

typedef unsigned int uint;
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

    void addCube(float scale_x = 1.f, float scale_y = 1.f, float scale_z = 1.f, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addSphere(uint resolution = 4, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addCapsule(uint resolution = 4, MeshData::Type type = DEFAULT_MESH_TYPE);
    void addIcosahedron(MeshData::Type type = DEFAULT_MESH_TYPE);
    void readFromObj(const char *path, MeshData::Type type = DEFAULT_MESH_TYPE);

    static std::vector<float> makeSingleArray(const std::vector<float> &positions, const std::vector<float> &normals,
                                              const std::vector<float> &texcoords, const MeshData::Type type);

  private:
    std::vector<uint> m_indices;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_uvs;
    MeshData::Type m_type = DEFAULT_MESH_TYPE;
};

#endif // MESHDATA_H
