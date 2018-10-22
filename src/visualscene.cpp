#include "visualscene.h"
#include "glad.h"
#include "shader.h"

#include "logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>

class ShaderManager {
    std::unique_ptr<Shader> flat;
    std::unique_ptr<Shader> PBR;
    // a lot of shaders including transparency/shadow/multilight enabled/disabled;
    ShaderManager() {
        flat.reset(new Shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl"));
        DLOG("Shaders initialized");
    }

  public:
    static ShaderManager *GetInstance() {
        static ShaderManager inst;
        return &inst;
    };
    Shader *flatShader() { return flat.get(); }
};

Mesh::Mesh(const MeshData &mData) {
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

void Mesh::render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_VertexCount, GL_UNSIGNED_INT, 0);
};

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
};

void Scene::addVisualModel(VisualModel *visualmodel) {
    m_visualmodels.push_back(visualmodel);
}

Scene::Scene() {
    ShaderManager::GetInstance(); // initialize all shaders
}

Scene::~Scene() {
}

//void VScene::renderAll() const {
//    // set view and projection
//    shader->setMat4(1, camera->getMatrix());
//    glm::mat4 projection(glm::perspective(camera->getFOV(), camera->getAspectRatio(), 0.1f, 10000.0f));
//    shader->setMat4(2, projection);
//    for (const auto &mesh : meshes)
//        if (mesh)
//            mesh->render();
//}

//void VScene::clear() {
//    glClearColor(0.10f, 0.1f, 0.13f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}

//void VScene::clearDepth() {
//    glClear(GL_DEPTH_BUFFER_BIT);
//}

size_t VisualModel::idCounter = 0;
VisualModel::VisualModel(Scene &scene) : m_scene(&scene), id(idCounter++) {
    scene.addVisualModel(this);
}

Material::Material() {
    shader = ShaderManager::GetInstance()->flatShader();
}

Material::Material(Color _color, float _metallic, float _roughness) : Material() {
}

void Camera::takeShot() {
    //for all objects in
}
