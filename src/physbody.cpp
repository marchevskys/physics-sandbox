#include "physbody.h"
#include "logger.h"
#include "meshdata.h"

#define _NEWTON_USE_DOUBLE
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <newton/Newton.h>

static const std::array<double, 16> Identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
double origin[3]{0, 0, 0};
// PHYSICS WORLD
PhysWorld::PhysWorld() {
    m_world = NewtonCreate();
    //DLOG(NewtonGetMaxThreadsCount(m_world));
    //THROW();
    NewtonSetThreadsCount(m_world, 4);
    NewtonSetNumberOfSubsteps(m_world, 8);
    DLOG("world created");
}

PhysWorld::~PhysWorld() {
    NewtonWaitForUpdateToFinish(m_world);
    NewtonDestroyAllBodies(m_world);
    NewtonDestroy(m_world);
    DLOG("world destroyed");
}

void PhysWorld::update(const double dt) {
    //NewtonUpdate(m_world, dt);
    NewtonUpdateAsync(m_world, dt);
}

// COLLISIONS
CollisionShape::CollisionShape(const PhysWorld &world) {}

CollisionShape::~CollisionShape() {
    if (m_collision)
        NewtonDestroyCollision(m_collision);
}

CollisionSphere::CollisionSphere(const PhysWorld &world, double radius) : CollisionShape(world) {
    m_collision = NewtonCreateSphere(world.get(), radius, 0, nullptr);
}

CollisionCuboid::CollisionCuboid(const PhysWorld &world, double w, double l, double h) : CollisionShape(world) {
    m_collision = NewtonCreateBox(world.get(), w, l, h, 0, nullptr);
}

CollisionIcosahedron::CollisionIcosahedron(const PhysWorld &world, double radius) : CollisionShape(world) {

    const auto vertices = MeshPrimitives::icosahedron().getVertices<double>();
    m_collision = NewtonCreateConvexHull(world.get(), vertices.size() / 3, vertices.data(), sizeof(double) * 3, /*tolerance*/ 0.0, 0, nullptr);
}

CollisionConvex::CollisionConvex(const PhysWorld &world, const std::vector<double> points) : CollisionShape(world) {
    m_collision = NewtonCreateConvexHull(world.get(), points.size() / 3, points.data(), sizeof(double) * 3, /*tolerance*/ 0.0, 0, nullptr);
}

// PHYSICS BODY
void setForcesAndTorques(const NewtonBody *const body, double timestep, int threadIndex) {
    double mass, Ixx, Iyy, Izz;
    NewtonBodyGetMass(body, &mass, &Ixx, &Iyy, &Izz);
    glm::dvec3 gravityForce{0, 0, -9.80665 * mass}; // G-force // gravity here
    PhysBody::Data *mydata = static_cast<PhysBody::Data *>(NewtonBodyGetUserData(body));
    glm::dvec3 speed;
    NewtonBodyGetVelocity(body, &speed[0]);
    glm::dvec3 aeroForce = -0.05 * glm::length(speed) * speed;
    for (int i = 0; i < 3; i++)
        mydata->force[i] += gravityForce[i] + aeroForce[i];
    NewtonBodySetForce(body, mydata->force);
    NewtonBodySetTorque(body, mydata->torque);
}

void updateTransformCallback(const NewtonBody *const body, const dFloat *const matrix, int threadIndex) {
    PhysBody::Data *mydata = static_cast<PhysBody::Data *>(NewtonBodyGetUserData(body));
    mydata->torque[0] = 0.0;
    mydata->torque[1] = 0.0;
    mydata->torque[2] = 0.0;
    mydata->force[0] = 0.0;
    mydata->force[1] = 0.0;
    mydata->force[2] = 0.0;
}

void PhysBody::turnOffDefaultResistance() {
    NewtonBodySetLinearDamping(m_body, 0.0);
    double angularDamping = 0.0;
    NewtonBodySetAngularDamping(m_body, &angularDamping);
}

PhysBody::PhysBody(const PhysWorld &world, CollisionShape &&shape, const double m, const double Ixx, const double Iyy, const double Izz) {
    //DLOG("Body created");
    std::array<double, 16> mat = Identity;
    for (int i = 0; i < 3; ++i)
        mat[12 + i] = origin[i];

    m_body = NewtonCreateDynamicBody(world.get(), shape.get(), mat.data());
    NewtonBodySetMassMatrix(m_body, m, Ixx, Iyy, Izz);
    NewtonBodySetForceAndTorqueCallback(m_body, setForcesAndTorques);
    NewtonBodySetTransformCallback(m_body, updateTransformCallback);
    //NewtonBodySetContinuousCollisionMode(m_body, 1);
    NewtonBodySetUserData(m_body, static_cast<void *>(&data));
    turnOffDefaultResistance();
}

PhysBody::PhysBody(const PhysBody &other) { THROW("TRY TO COPY PHYSBODY"); }

void PhysBody::setMassMatrix(const double *mass) { NewtonBodySetMassMatrix(m_body, mass[0], mass[1], mass[2], mass[3]); }

PhysBody::PhysBody(PhysBody &&other) noexcept {
    m_body = other.m_body;
    other.m_body = nullptr;
    data = other.data;
}

void PhysBody::setPos(const double *pos) {
    auto mat = Identity;
    for (int i = 0; i < 3; ++i)
        mat[12 + i] += pos[i];
    NewtonBodySetMatrix(m_body, mat.data());
}

void PhysBody::setVelocity(const double *velocity) { NewtonBodySetVelocity(m_body, velocity); }

void PhysBody::addForce(const double *force) {
    for (int i = 0; i < 3; ++i)
        data.force[i] += force[i];
}

void PhysBody::addForce(const double x, const double y, const double z) {
    data.force[0] += x;
    data.force[1] += y;
    data.force[2] += z;
}

void PhysBody::addTorque(const double *torque) {
    for (int i = 0; i < 3; ++i)
        data.torque[i] += torque[i];
}

void PhysBody::addTorque(const double x, const double y, const double z) {
    data.torque[0] += x;
    data.torque[1] += y;
    data.torque[2] += z;
}

void PhysBody::setOmega(const double *omega) { NewtonBodySetOmega(m_body, omega); }

void PhysBody::getPos(double *pos) { NewtonBodyGetPosition(m_body, pos); }

void PhysBody::getMatrix(double *mat) const { NewtonBodyGetMatrix(m_body, mat); }

PhysBody::~PhysBody() {
    if (m_body) {
        NewtonDestroyBody(m_body);
        //DLOG("Body destroyed");
    }
}

void PhysBody::setOrigin(double originX, double originY, double originZ) {
    origin[0] = originX;
    origin[1] = originY;
    origin[2] = originZ;
}
