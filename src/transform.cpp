#include "transform.h"
#include "logger.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Transform::Transform() { m_transform = glm::dmat4(1); }

Transform::Transform(glm::dvec3 pos) : Transform() {
    setPos(pos);
}

void Transform::setPos(glm::dvec3 pos) {
    m_transform[3][0] = pos[0];
    m_transform[3][1] = pos[1];
    m_transform[3][2] = pos[2];
}

void Transform::offsetPos(glm::dvec3 pos) {
    //m_transform = glm::translate(m_transform, pos);
    m_transform[3][0] += pos[0];
    m_transform[3][1] += pos[1];
    m_transform[3][2] += pos[2];
}

glm::dmat4 Transform::getInverse() {
    return glm::inverse(m_transform);
}
