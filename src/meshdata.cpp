#include "meshdata.h"
#include "logger.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>

std::vector<float> MeshData::makeSingleArray(const std::vector<float> &positions, const std::vector<float> &normals, const std::vector<float> &texcoords,
                                             const MeshData::Type type) {
    std::vector<float> array;

    switch (type) {
    case MeshData::Type::V:
        array = positions;
        break;

    case MeshData::Type::VN:
        if (positions.size() != normals.size())
            THROW("VN");
        array.reserve(positions.size() + normals.size());
        for (size_t i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(normals[i]);
        }
        break;

    case MeshData::Type::VT:
        if (positions.size() != texcoords.size())
            THROW("VT");
        array.reserve(positions.size() + texcoords.size());
        for (size_t i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(texcoords[i]);
        }
        break;

    case MeshData::Type::VTN:
        if (positions.size() != normals.size() || positions.size() != texcoords.size() / 2 * 3) {
            THROW("VTN");
        }
        array.reserve(positions.size() + normals.size() + texcoords.size());
        for (size_t i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(texcoords[i]);
            array.push_back(normals[i]);
        }

        break;
    //case MeshData::Type::VTNTB:
    //break;
    default:
        THROW("Bad mesh type");
    }

    return array;
}

MeshData::MeshData(MeshData &&other) {
    m_indices = std::move(other.m_indices);
    m_vertices = std::move(other.m_vertices);
    m_normals = std::move(other.m_normals);
    m_uvs = std::move(other.m_uvs);
    m_type = other.m_type;
}

MeshData::MeshData(const MeshData &other) {
    THROW("copied");
}

void MeshData::scale(float k) {
    for (auto &v : m_vertices)
        v *= k;
}

template <>
const std::vector<float> MeshData::getVertices() { return m_vertices; }

template <>
const std::vector<double> MeshData::getVertices() {
    std::vector<double> vec(m_vertices.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = static_cast<double>(m_vertices[i]);
    }
    return vec;
}

MeshData MeshPrimitives::icosahedron(MeshData::Type type) {
    MeshData data;
    std::vector<uint> indices{
        2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 1, 5, 0, 11, 6, 7, 11, 7,
        8, 11, 8, 9, 11, 9, 10, 11, 10, 6, 1, 2, 6, 2, 3, 7, 3, 4, 8, 4,
        5, 9, 5, 1, 10, 2, 7, 6, 3, 8, 7, 4, 9, 8, 5, 10, 9, 1, 6, 10};

    std::vector<float> positions{
        0.000f, 0.000f, 1.000f,
        0.894f, 0.000f, 0.447f,
        0.276f, 0.851f, 0.447f,
        -0.724f, 0.526f, 0.447f,
        -0.724f, -0.526f, 0.447f,
        0.276f, -0.851f, 0.447f,
        0.724f, 0.526f, -0.447f,
        -0.276f, 0.851f, -0.447f,
        -0.894f, 0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
        0.724f, -0.526f, -0.447f,
        0.000f, 0.000f, -1.000f};

    data.m_vertices = std::move(positions);
    data.m_indices = std::move(indices);
    return data;
}

MeshData MeshPrimitives::sphere(uint resolution, MeshData::Type type) {
#define F(x) static_cast<float>(x)

    const float radius = 1.f;
    const uint numMeridian = resolution * 2; // resolution of mesh
    const uint numParallel = resolution;     // resolution of mesh
    float parralelDivider = 2.0f * F(M_PI) / (F(numMeridian));
    float meridianDivider = F(M_PI) / (F(numParallel)) * .9999f;

    std::vector<glm::vec2> parallel(numMeridian + 1);
    for (uint i = 0; i <= numMeridian; i++)
        parallel[i] = glm::vec2(cos(F(i) * parralelDivider), sin(F(i) * parralelDivider)) * radius; // zero circle

    std::vector<float> positions, normals, uvs;
    positions.reserve((numParallel + 1) * (numMeridian + 1));
    for (uint j = 0; j <= numParallel; j++)
        for (uint i = 0; i <= numMeridian; i++) {
            auto p = parallel[i] * sin(F(j) * meridianDivider);
            //positions.emplace_back(p[0], p[1], radius * cos(F(j) * meridianDivider));
            positions.insert(positions.end(), {p[0], p[1], radius * cos(F(j) * meridianDivider)});
        }

    std::vector<uint> indices;
    for (uint i = 0; i < numMeridian; i++)
        for (uint j = 0; j < numParallel; j++) {
            auto offset = j * (numMeridian + 1) + i;
            auto offset_j = (j + 1) * (numMeridian + 1) + i;
            auto offset_i = j * (numMeridian + 1) + i + 1;
            auto offset_i_j = (j + 1) * (numMeridian + 1) + i + 1;
            indices.insert(indices.end(), {offset, offset_j, offset_i});
            indices.insert(indices.end(), {offset_j, offset_i, offset_i_j});
        }
    MeshData data;
    data.m_vertices = std::move(positions);
    data.m_indices = std::move(indices);
    data.m_type = type;
    //    std::vector<glm::vec2> uvs;
    //    uvs.reserve(positions.size());
    //    for (int i = 0; i <= numMeridian; i++)
    //        for (int j = 0; j <= numParallel; j++) {
    //            positions.emplace_back(F(i) / F(numMeridian), F(j) / F(numParallel));
    //        }
    return data;
}

MeshData MeshPrimitives::cube(float scale_x, float scale_y, float scale_z, MeshData::Type type) {
    std::vector<float> positions{
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f};

    std::vector<float> normals{
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    std::vector<float> texcoords = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    for (size_t i = 0; i < positions.size(); i += 3) {
        positions[i] *= scale_x;
        positions[i + 1] *= scale_y;
        positions[i + 2] *= scale_z;
    }
    MeshData data;
    //result.m_vertexArray = makeSingleArray(positions, normals, texcoords, type);
    data.m_vertices = std::move(positions);
    data.m_indices.resize(36);
    data.m_type = type;
    for (uint i = 0; i < 36; i++)
        data.m_indices[i] = i;

    return data;
}
