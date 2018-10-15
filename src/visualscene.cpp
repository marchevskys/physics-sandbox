#include "visualscene.h"
#include "glad.h"
#include "shader.h"

#include "logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>
class Camera {
    float m_AspectRatio = 1.f;
    float m_FieldOfView = 65.f;
    float m_NearClip = 0.1f;
    float m_FarClip = 10000.0f;
    double *m_ViewMatrix;

  public:
    Camera(double *_matrix, float _fov, float _ar) : m_ViewMatrix(_matrix), m_AspectRatio(_ar), m_FieldOfView(_fov) {
    }
    double *getMatrix() { return m_ViewMatrix; }

    float getAspectRatio() { return m_AspectRatio; }
    void setAspectRatio(float _ar) { m_AspectRatio = _ar; }

    void attachMatrix(double *_mat);
    void setFOV(float _fov);
    float getFOV() { return m_FieldOfView; }
    ~Camera() {}
};

class Mesh {
    friend class VScene;
    unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;
    unsigned int m_VertexCount = 0;
    double *m_matrix;
    Shader *m_shader;
    bool operator==(const Mesh &other) { return m_VAO == other.m_VAO; }

  public:
    Mesh(const MeshData &mData, Shader *_shader, double *_matrix)
        : m_shader(_shader), m_matrix(_matrix) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, mData.m_vertexArray.size() * sizeof(GLfloat),
                     mData.m_vertexArray.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mData.m_indexArray.size() * sizeof(GLuint),
                     mData.m_indexArray.data(), GL_STATIC_DRAW);
        m_VertexCount = mData.m_indexArray.size();

        switch (mData.m_type) {
        case MeshData::Type::V: {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                                  (GLvoid *)(0 * sizeof(GLfloat)));
            glEnableVertexAttribArray(0);

        } break;

        case MeshData::Type::VT: {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                                  (GLvoid *)(0 * sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        } break;

        case MeshData::Type::VN: {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                  (GLvoid *)(0 * sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        } break;

        case MeshData::Type::VTN: {
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

        case MeshData::Type::VTNB: {
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
        default: {
            DLOGN("Bad mesh type");
            throw("");
        }
        }
        // note that this is allowed, the call to glVertexAttribPointer registered
        // VBO as the vertex attribute's bound vertex buffer object so afterwards we
        // can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    };
    void render() {
        //shader->use();
        m_shader->setMat4(0, m_matrix);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_VertexCount, GL_UNSIGNED_INT, 0);
    };
    ~Mesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    };
};

class MeshLOD {
    std::vector<std::pair<Mesh, double>> lods;

  public:
    void render(double dist) {
        for (auto it = lods.end() - 1; it >= lods.begin(); --it) {
            if (dist < it->second) {
                it->first.render();
                break;
            }
        }
    }
};

VScene::VScene() {
    createDefaultShaders();
}

void VScene::createDefaultShaders() {
    shader = new Shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");
    shader->use();
}

void VScene::renderAll() const {
    // set view and projection
    shader->setMat4(1, camera->getMatrix());
    glm::mat4 projection(glm::perspective(camera->getFOV(), camera->getAspectRatio(), 0.1f, 10000.0f));
    shader->setMat4(2, projection);
    for (const auto &mesh : meshes)
        if (mesh)
            mesh->render();
}

void VScene::clear() {
    glClearColor(0.10f, 0.1f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void VScene::clearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

Mesh *VScene::createMesh(const MeshData &data, double *matrix) {
    Mesh *mesh = new Mesh(data, shader, matrix);

    meshes.push_back(mesh);
    return mesh;
}

void VScene::deleteMesh(Mesh *mesh) {
    delete mesh;
    mesh = nullptr;
}

Camera *VScene::createCamera(double *_matrix, float _fov, float _ar) {
    camera = new Camera(_matrix, _fov, _ar);
    return camera;
}

void VScene::setCameraAR(float _ar) {
    camera->setAspectRatio(_ar);
}

size_t VScene::deleteAllCameras() {
    delete camera;
    camera = nullptr;
    return 1;
}

size_t VScene::deleteAllShaders() {
    delete shader;
    shader = nullptr;
    return 1; // number of deletes shaders, now it is one
}

size_t VScene::deleteAllMeshes() {
    for (auto &mesh : meshes)
        if (mesh)
            delete mesh;
    size_t numOfDeletedMeshes = meshes.size();
    meshes.clear();
    return numOfDeletedMeshes;
}

VScene::~VScene() {
    deleteAllCameras();
    deleteAllMeshes();
    deleteAllShaders();
}
