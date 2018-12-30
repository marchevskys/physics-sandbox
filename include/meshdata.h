#ifndef MESHDATA_H
#define MESHDATA_H
#define DEFAULT_MESH_TYPE MeshData::Type::V

#include <array>
#include <vector>

typedef unsigned int uint;
class MeshData {
    friend class Mesh;
    friend struct MeshPrimitives;

  public:
    //clang-format off
    enum class Type { UNDEF,
                      V,
                      VN,
                      VT,
                      VTN,
                      VTNB }; /*vertex, texture, normal, binormal*/
                              //clang-format on
    MeshData() {}
    MeshData(MeshData &&other);
    MeshData(const MeshData &other);
    void scale(float k);
    static std::vector<float> makeSingleArray(const std::vector<float> &positions, const std::vector<float> &normals,
                                              const std::vector<float> &texcoords, const MeshData::Type type);
    template <class T>
    const std::vector<T> getVertices();

  private:
    std::vector<uint> m_indices;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_uvs;
    MeshData::Type m_type = DEFAULT_MESH_TYPE;
};

struct MeshPrimitives {
    static MeshData sphere(uint resolution = 4, MeshData::Type type = DEFAULT_MESH_TYPE);
    static MeshData cube(float scale_x = 1.f, float scale_y = 1.f, float scale_z = 1.f, MeshData::Type type = DEFAULT_MESH_TYPE);
    static MeshData icosahedron(MeshData::Type type = DEFAULT_MESH_TYPE);
};

#endif // MESHDATA_H
