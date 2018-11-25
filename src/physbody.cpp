#include "physbody.h"
#include "logger.h"

#define _NEWTON_USE_DOUBLE
#include <newton/Newton.h>

static const double Identity[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
// PHYSICS WORLD
PhysWorld::PhysWorld() {
    m_world = NewtonCreate();
    NewtonSetThreadsCount(m_world, 4);
    NewtonSetNumberOfSubsteps(m_world, 1);
    DLOG("world created");
}

PhysWorld::~PhysWorld() {
    NewtonDestroyAllBodies(m_world);
    NewtonDestroy(m_world);
    DLOG("world destroyed");
}
void PhysWorld::update(const double dt) { NewtonUpdate(m_world, dt); }

// COLLISIONS
CollisionShape::CollisionShape(const PhysWorld &world) {
    DLOG("collision created");
    m_collision = NewtonCreateNull(world.get());
}

CollisionShape::~CollisionShape() {
    DLOG("collision destoyed");
    NewtonDestroyCollision(m_collision);
}

CollisionSphere::CollisionSphere(const PhysWorld &world, double radius) : CollisionShape(world) {
    m_collision = NewtonCreateSphere(world.get(), radius, 0, nullptr);
}

CollisionCuboid::CollisionCuboid(const PhysWorld &world, double w, double l, double h) : CollisionShape(world) {
    m_collision = NewtonCreateBox(world.get(), w, l, h, 0, nullptr);
}

// PHYSICS BODY
void setForcesAndTorques(const NewtonBody *const body, double timestep, int threadIndex) {
    double mass, Ixx, Iyy, Izz;
    NewtonBodyGetMass(body, &mass, &Ixx, &Iyy, &Izz);
    double force[3] = {0, 0, -9.80665 * mass}; // G-force // gravity here

    PhysBody::Data *mydata = static_cast<PhysBody::Data *>(NewtonBodyGetUserData(body));
    for (int i = 0; i < 3; i++)
        force[i] += mydata->force[i];
    NewtonBodySetForce(body, force);
    NewtonBodySetTorque(body, mydata->torque);
    double pos[4];
    NewtonBodyGetPosition(body, pos);
}

PhysBody::PhysBody(const PhysWorld &world, CollisionShape &&shape, const double m, const double Ixx, const double Iyy, const double Izz) {
    DLOG("Body created");
    m_body = NewtonCreateDynamicBody(world.get(), shape.get(), Identity);
    NewtonBodySetMassMatrix(m_body, m, Ixx, Iyy, Izz);
    NewtonBodySetForceAndTorqueCallback(m_body, setForcesAndTorques);
    NewtonBodySetUserData(m_body, static_cast<void *>(&data));
}

PhysBody::PhysBody(const PhysBody &other) {
    THROW("TRYING TO COPY PHYSBODY");
}

PhysBody::PhysBody(PhysBody &&other) noexcept {
    m_body = other.m_body;
    other.m_body = nullptr;
}

void PhysBody::setVelocity(const double *velocity) { NewtonBodySetVelocity(m_body, velocity); }
void PhysBody::setOmega(const double *omega) { NewtonBodySetOmega(m_body, omega); }

void PhysBody::getMatrix(double *mat) const { NewtonBodyGetMatrix(m_body, mat); }

PhysBody::~PhysBody() {
    if (m_body) {
        NewtonDestroyBody(m_body);
        DLOG("Body destroyed");
    }
}
