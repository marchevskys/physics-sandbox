#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
class Camera
{
    glm::dvec3 m_pos, m_dir, m_up;
    float m_near = 0.1f, m_far = 10000.f, m_fov = 1.f, m_aspectRatio = 1.f;
    glm::mat4 m_projection;

    void updateProjection();
public:
    Camera(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 up = {0.0, 0.0, 1.0});

    void set(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 up = {0.0, 0.0, 1.0}) { m_pos = pos, m_dir = dir, m_up = up; }
    void setFOV(float fov);
    void setAspectRatio(float ar);
    void move(glm::dvec3 deltaDir);
    void rotate(glm::dvec3 deltaPhi);

    glm::dmat4 getView();
    const glm::mat4& getProjection(){ return m_projection; }
};

#endif // CAMERA_H
