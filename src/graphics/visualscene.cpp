#include "visualscene.h"
//#include "glad.h"

#include "shader.h"
#include "mesh.h"
#include "../logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>

namespace Graphics {
class ShaderManager {
    //std::unique_ptr<Shader> flat;
    std::vector<Shader> shaders;
    ShaderManager() {
        shaders.emplace_back("../GameTest/src/shaders/vertex.glsl", "../GameTest/src/shaders/fragment.glsl");
        DLOG("Shaders initialized");
    }

  public:
    static ShaderManager *GetInstance() {
        static ShaderManager inst;
        return &inst;
    }
    const Shader * flatShader() const { return &shaders[0]; }
    const std::vector<Shader> &getAll3DShaders() { return shaders; }
};

class MeshManager {
    MeshManager() {}
    ~MeshManager() {}

  public:
    MeshManager *GetInstance() {
        static MeshManager manager;
        return &manager;
    }
};


void Scene::addVisualModel(VisualModel *visualmodel) {
    m_visualmodels.push_back(visualmodel);
}

Scene::Scene() : allShaders(ShaderManager::GetInstance()->getAll3DShaders()) {

    m_visualmodels.reserve(100);
    DLOG("scene created");
}

void Scene::render(Camera &cam) const {
    glm::mat4 projection(glm::perspective(glm::radians(cam.m_FieldOfView), cam.m_AspectRatio, cam.m_NearClip, cam.m_FarClip));
    for (auto &s : allShaders) {
        s.setMat4(1, cam.m_matrix);
        s.setMat4(2, glm::value_ptr(projection));
    }
    for (const auto &model : m_visualmodels) {
        if (model) {
            const auto &modelMatrix = model->m_matrix;
            const auto &shader = model->m_material->shader;
            shader->use();
            shader->setMat4(0, modelMatrix);
            model->m_mesh->render();
        }
    }
}

Scene::~Scene() {
}


Material::Material() {
    shader = ShaderManager::GetInstance()->flatShader();
}

Material::Material(Color _color, float _metallic, float _roughness) : Material() {
}

VisualModel::VisualModel(Scene *scene, Mesh *mesh,
                         Material *material, scalar * matrix) : m_scene(scene), m_mesh(mesh), m_material(material), m_matrix(matrix) {
    scene->addVisualModel(this);
    DLOG("VisualModel created");
}



VisualModel::~VisualModel() {
}

Camera::Camera(Scene *scene) : m_scene(scene) {
}

void Camera::setMatrix(scalar *matrix) {
    m_matrix = matrix;
}

void Camera::setFOV(float fov) {
    m_FieldOfView = fov;
}

Camera::~Camera() {
}
} // namespace Graphics
