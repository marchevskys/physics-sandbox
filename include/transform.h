#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Transform {
    glm::dmat4 m_transform;

  public:
    Transform();
    Transform(glm::dvec3 pos);
    void setPos(glm::dvec3 pos);
    void offsetPos(glm::dvec3 pos);
    glm::dmat4 &get() { return m_transform; }
    glm::dvec3 getPos() { return m_transform[3]; };
    //double* get(){ return glm::value_ptr(m_transform); }
    void set(double *mat) { memcpy(&m_transform[0][0], mat, sizeof(glm::dmat4)); }
    glm::dmat4 getInverse();
};

#endif // TRANSFORM_H
