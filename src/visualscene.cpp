#include "visualscene.h"
#include "glad.h"

#include "logger.h"

class Mesh {
    //public:
    friend class VisualScene;
    Mesh();
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int vertexCount = 0;
    bool operator==(const Mesh &other) { return VAO == other.VAO; }
};

VisualScene::VisualScene() {
    createDefaultShaders();
}

void VisualScene::renderAll() const {
    for (const auto &mesh : meshes)
        if (mesh)
            renderMesh(mesh);
}

void VisualScene::renderMesh(Mesh *m) const {
    glBindVertexArray(m->VAO);
    glDrawElements(GL_TRIANGLES, m->vertexCount, GL_UNSIGNED_INT, 0);
}

void VisualScene::clear() {
    glClearColor(0.10f, 0.1f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void VisualScene::createDefaultShaders() {
}

Mesh *VisualScene::createMesh(const std::vector<float> &pointData, const std::vector<unsigned int> &indexData, MeshType type) {
    Mesh *mesh = new Mesh();
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(GLfloat),
                 pointData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint),
                 indexData.data(), GL_STATIC_DRAW);
    mesh->vertexCount = indexData.size();
    switch (type) {
    case MeshType::V: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                              (GLvoid *)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);

    } break;

    case MeshType::VT: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                              (GLvoid *)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    } break;

    case MeshType::VN: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                              (GLvoid *)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    } break;

    case MeshType::VTN: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    } break;

    case MeshType::VTNB: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                              (GLvoid *)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                              (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                              (GLvoid *)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                              (GLvoid *)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    } break;
    default:
        throw("Bad mesh type");
        break;
    }
    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    meshes.push_back(mesh);
    return mesh;
}

void VisualScene::deleteMesh(Mesh *mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    delete mesh;
    mesh = nullptr;
}

size_t VisualScene::deleteAllMeshes() {
    for (auto &mesh : meshes)
        if (mesh)
            delete mesh;
    size_t numOfDeletedMeshes = meshes.size();
    meshes.clear();
    return numOfDeletedMeshes;
}
