#include "meshdata.h"
#include "logger.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>
std::vector<float> MeshData::makeSingleArray(const std::vector<float> &positions, const std::vector<float> &normals, const std::vector<float> &texcoords,
                                             const MeshData::Type type) {
    std::vector<float> array;
    std::string error(std::string(__FUNCTION__) + " Arrays does not match: ");

    switch (type) {
    case MeshData::Type::V:
        array = positions;
        break;

    case MeshData::Type::VN:
        if (positions.size() != normals.size())
            throw(error + "VN");
        array.reserve(positions.size() + normals.size());
        for (int i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(normals[i]);
        }
        break;

    case MeshData::Type::VT:
        if (positions.size() != texcoords.size())
            throw(error + "VT");
        array.reserve(positions.size() + texcoords.size());
        for (int i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(texcoords[i]);
        }
        break;

    case MeshData::Type::VTN:
        if (positions.size() != normals.size() || positions.size() != texcoords.size() / 2 * 3) {
            throw(error + "VTN");
        }
        array.reserve(positions.size() + normals.size() + texcoords.size());
        for (int i = 0; i < positions.size(); ++i) {
            array.push_back(positions[i]);
            array.push_back(texcoords[i]);
            array.push_back(normals[i]);
        }

        break;
    //case MeshData::Type::VTNTB:
    //break;
    default:
        throw("Bad mesh type");
    }

    return array;
}

void MeshData::addCube(float scale_x, float scale_y, float scale_z, MeshData::Type type) {

    std::vector<float> positions{
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};
    std::vector<float> normals{
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
    std::vector<float> texcoords = {
        // positions
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f};

    for (int i = 0; i < positions.size(); i += 3) {
        positions[i] *= scale_x;
        positions[i + 1] *= scale_y;
        positions[i + 2] *= scale_z;
    }

    //result.m_vertexArray = makeSingleArray(positions, normals, texcoords, type);
    m_vertexArray = positions;
    m_indexArray.resize(36);
    m_type = type;
    for (int i = 0; i < 36; i++)
        m_indexArray[i] = i;
}

void MeshData::addSphere(uint resolution, MeshData::Type type) {
#define F(x) static_cast<float>(x)

    const float radius = 1.f;
    const int numMeridian = resolution * 2; // resolution of mesh
    const int numParallel = resolution;     // resolution of mesh
    float parralelDivider = 2.0f * M_PI / (F(numMeridian));
    float meridianDivider = M_PI / (F(numParallel)) * .9999;

    std::vector<glm::vec2> parallel(numMeridian + 1);
    for (int i = 0; i <= numMeridian; i++)
        parallel[i] = glm::vec2(cos(F(i) * parralelDivider), sin(F(i) * parralelDivider)) * radius; // zero circle

    std::vector<float> positions, normals, uvs;
    positions.reserve((numParallel + 1) * (numMeridian + 1));
    for (int j = 0; j <= numParallel; j++)
        for (int i = 0; i <= numMeridian; i++) {
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
            indices.insert(indices.end(), {offset_i, offset_j, offset_i_j});
        }

    m_vertexArray = std::move(positions);
    m_indexArray = std::move(indices);
    //    std::vector<glm::vec2> uvs;
    //    uvs.reserve(positions.size());
    //    for (int i = 0; i <= numMeridian; i++)
    //        for (int j = 0; j <= numParallel; j++) {
    //            positions.emplace_back(F(i) / F(numMeridian), F(j) / F(numParallel));
    //        }
}

void MeshData::addIcosahedron(MeshData::Type type) {
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

    m_vertexArray = std::move(positions);
    m_indexArray = std::move(indices);
}
