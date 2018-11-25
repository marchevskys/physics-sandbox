#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::dmat4 Camera::getView() {
    return glm::lookAt(m_pos, m_dir, m_up);
}

void Camera::updateProjection() { m_projection = glm::perspective(m_fov, m_aspectRatio, m_near, m_far); }

Camera::Camera(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 up) {
    set(pos, dir, up);
    updateProjection();
}

void Camera::setFOV(float fov) {
    m_fov = fov;
    updateProjection();
}

void Camera::setAspectRatio(float ar) {
    m_aspectRatio = ar;
    updateProjection();
}

void Camera::move(glm::dvec3 deltaDir) {
    m_pos += deltaDir;
    m_dir += deltaDir;
}

void Camera::rotate(glm::dvec3 deltaPhi) {
}
