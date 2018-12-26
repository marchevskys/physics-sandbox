#include "model.h"
#include "logger.h"
#include "mesh.h"
#include "meshdata.h"
#include "shader.h"
#include "shadermanager.h"
#include <vector>

Model::Model() {
}

Model::Model(const Mesh *mesh) {
    m_mesh = const_cast<Mesh *>(mesh);
}

Model::Model(const Mesh *mesh, const Material *material) {
    m_mesh = const_cast<Mesh *>(mesh);
    m_material = const_cast<Material *>(material);
}

void Model::render(const double *transform) {
    m_material->m_shader->use();
    m_material->m_shader->setMat4(0, transform);
    m_mesh->render();
}

Material::Material() : r(0.5), g(0.5), b(0.5) {
    m_shader = ShaderManager::get()->flatShader();
}
