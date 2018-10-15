#include "scene.h"
#include "glad.h"
#include "shader.h"

#include "logger.h"

#include <list>
#include <map>
#include <vector>

// VMesh, VScene are internal types
class VMesh;
class VScene;
class VMaterial;
//std::vector<Shader *> shaders;
Shader *shader = nullptr;

typedef std::vector<VMesh *> MeshContainer;

//class VShader {
//    VShader() = delete;
//    ~VShader() = delete;

//  public:
//    static Shader *getInstance() {
//        static Shader shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");
//        return &shader;
//    }
//};
void createDefaultShaders() {
    if (!shader)
        shader = new Shader("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");
}
///////////// MESH //////////////
class VMesh {
    friend class VScene;
    //VScene *m_parentScene = nullptr;
    Material *material;
    scalar m_matrix[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;
    unsigned int m_VertexCount = 0;

  public:
    VMesh(const uint arrayLength, const float *arrayData, const uint indexLength, const uint *indexData, const MeshType type) {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s).
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, arrayLength * sizeof(GLfloat),
                     arrayData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength * sizeof(GLuint),
                     indexData, GL_STATIC_DRAW);
        m_VertexCount = indexLength;

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
    }
    ~VMesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
    void render() {
        //shader->use();
        shader->setMat4(0, m_matrix);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_VertexCount, GL_UNSIGNED_INT, 0);
    }
};
///////////// SCENE //////////////
class VScene {
  public:
    MeshContainer meshes;
    VScene() {
        createDefaultShaders();
    }
    ~VScene() {
        for (MeshContainer::iterator it = meshes.begin(); it != meshes.end(); ++it)
            delete *it;
    }
};

Scene *createScene() {
    Scene *scene = (Scene *)new VScene;
    return scene;
}

void destroyScene(Scene *scene) {
    VScene *const vscene = (VScene *)scene;
    delete vscene;
}

void render(const Scene *scene) {
    VScene *const vscene = (VScene *)vscene;
    for (MeshContainer::iterator it = vscene->meshes.begin(); it != vscene->meshes.end(); ++it) {
        if (*it) {
            // render mesh
        }
    }
}

Mesh *createMesh(const Scene *scene, const uint arrayLength, const float *arrayData, const uint indexLength, const uint *indexData, const MeshType type) {
    VScene *const vscene = (VScene *)scene;
    VMesh *vmesh = new VMesh(arrayLength, arrayData, indexLength, indexData, type);

    vscene->meshes.push_back(vmesh);
    return (Mesh *)vmesh;
}

void destroyMesh(Mesh *mesh) {
    delete (VMesh *)mesh;
    mesh = nullptr;
    // find in all arrays
    // delete and make it zero
    // if number of deletions > 1000 -> shrink to fit array
}
///////////// MATERIAL //////////////

class VMaterial {
  public:
};

Material *createMaterial() {
    //createDefaultShaders();
    return nullptr;
}
