#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>
Transform::Transform() { m_transform = glm::dmat4(1); }

Transform::Transform(glm::dvec3 pos) : Transform() {
    glm::translate(m_transform, pos);
}

glm::dmat4 Transform::getInverse() {
    return glm::inverse(m_transform);
}
