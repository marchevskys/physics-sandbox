#include "physbody.h"

#include "logger.h"

#include <Newton.h>
#include <algorithm>
#include <float.h>
#include <thread>

//namespace Physics {
bool prevSleepState = 0;
static int step = 0;
//NewtonWorld* PhysBody::world = nullptr;
static NewtonWorld *world = nullptr;
std::array<dFloat, 3> PhysBody::origin{0, 0, 0};

const std::array<dFloat, 16> identityMatrix = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};

CollisionShape::CollisionShape() {
    DLOG("collision constructed");
    m_collision = NewtonCreateNull(world);
}

CollisionShape::~CollisionShape() {
    DLOG("collision destructed");
    NewtonDestroyCollision(m_collision); // WARNING IT MAY DESTROY OBJECT'S COLLISION
}

CollisionSphere::CollisionSphere(dFloat radius) {
    m_collision = NewtonCreateSphere(world, radius, 0, nullptr);
}

CollisionCuboid::CollisionCuboid(dFloat w, dFloat l, dFloat h) {
    m_collision = NewtonCreateBox(world, w, l, h, 0, nullptr);
}

PhysBody::PhysBody(CollisionShape &&shape, std::array<dFloat, 4> massMatrix) {
    auto createMatrix = identityMatrix;
    for (int i = 0; i < 3; i++)
        createMatrix[i + 12] += origin[i];

    DLOG("ORIGIN", origin[0], origin[1], origin[2]);
    m_body = NewtonCreateDynamicBody(world, shape.m_collision, createMatrix.data());
    if (massMatrix[0] != 0.0)
        NewtonBodySetMassMatrix(m_body, massMatrix[0], massMatrix[1], massMatrix[2], massMatrix[3]);
    NewtonBodySetForceAndTorqueCallback(m_body, setForcesAndTorques);
    turnOffDefaultResistance();
    //NewtonBodySetContinuousCollisionMode(m_body, 1);      // for very high speed objects
    NewtonBodySetUserData(m_body, static_cast<void *>(&data));

    DLOG("Object created.");
}

void PhysBody::setForcesAndTorques(const NewtonBody *const body, dFloat timestep, int threadIndex) {
    dFloat mass, Ixx, Iyy, Izz;
    NewtonBodyGetMass(body, &mass, &Ixx, &Iyy, &Izz);
    dFloat force[3] = {0, 0, -9.80665 * mass}; // G-force // gravity here

    Data *mydata = static_cast<Data *>(NewtonBodyGetUserData(body));
    for (int i = 0; i < 3; i++)
        force[i] += mydata->force[i];
    NewtonBodySetForce(body, force);
    NewtonBodySetTorque(body, mydata->torque);
    dFloat pos[4];
    NewtonBodyGetPosition(body, pos);

    //    if (NewtonBodyGetID(body) == 2) {
    //        std::cout.precision(5);
    //        DLOG("step", step++, " sleep: ", NewtonBodyGetSleepState(body), pos[0], pos[1], pos[2]);
    //    }
}

void PhysBody::turnOffDefaultResistance() {
    NewtonBodySetLinearDamping(m_body, 0.0);
    dFloat angularDamping = 0.0;
    NewtonBodySetAngularDamping(m_body, &angularDamping);
}

void PhysBody::updatePhysics(const dFloat deltaTime) {
    NewtonUpdate(world, deltaTime);
}

PhysBody::~PhysBody() {
    if (world) {
        NewtonDestroyBody(m_body);
        DLOG("object destroyed");
    }
}

std::array<dFloat, 4> PhysBody::momentSphere(dFloat mass, dFloat radius, dFloat shellNess) {
    dFloat Ixx, Iyy, Izz;
    constexpr dFloat kShell(2.0 / 3.0);
    constexpr dFloat kSolid(2.0 / 5.0);
    dFloat I = (kShell * shellNess + kSolid * (1 - shellNess)) * mass * radius * radius;
    return {mass, I, I, I};
}

std::array<dFloat, 4> PhysBody::momentCuboid(dFloat mass, dFloat w, dFloat l, dFloat h, dFloat shellNess) {
    constexpr dFloat kSolid(1.0 / 12.0);
    constexpr dFloat kShell(1.0 / 6.0);
    dFloat k = (kShell * shellNess + kSolid * (1 - shellNess));
    dFloat Ixx = k * mass * (l * l + h * h);
    dFloat Iyy = k * mass * (w * w + h * h);
    dFloat Izz = k * mass * (w * w + l * l);
    return {mass, Ixx, Iyy, Izz};
}

void PhysBody::setForce(std::array<dFloat, 3> force) /// TODO force
{
    data.force[0] = force[0];
    data.force[1] = force[1];
    data.force[2] = force[2];
}

void PhysBody::getMatrix(std::array<dFloat, 16> values) {
    NewtonBodyGetMatrix(m_body, values.data());
}

void PhysBody::getMatrix(std::array<float, 16> fValues) {
    std::array<double, 16> values;
    NewtonBodyGetMatrix(m_body, values.data());
    for (int i = 0; i < 16; i++)
        fValues[i] = static_cast<float>(values[i]);
}

void PhysBody::getMatrix(double *values) {
    NewtonBodyGetMatrix(m_body, values);
}

void PhysBody::getMatrix(float *values) {
    double dValues[16];
    NewtonBodyGetMatrix(m_body, dValues);
    for (int i = 0; i < 16; i++)
        values[i] = static_cast<float>(dValues[i]);
}

void PhysBody::getPosition(std::array<dFloat, 3> values) {
    NewtonBodyGetPosition(m_body, values.data());
}

void PhysBody::getVelocity(std::array<dFloat, 3> velocity) {
    NewtonBodyGetVelocity(m_body, velocity.data());
}

void PhysBody::createWorld() {
    if (!world) {
        uint numOfCores = std::thread::hardware_concurrency();
        numOfCores = numOfCores > 1 ? numOfCores - 1 : 1;
        world = NewtonCreate();
        NewtonSetThreadsCount(world, numOfCores);

        //NewtonSelectBroadphaseAlgorithm(world, 10);

        //NewtonSetNumberOfSubsteps(world, 10);
        DLOG("world created, num of cores: ", numOfCores);
        DLOG("precision: ", (sizeof(dFloat) == 8 ? "dFloat" : "float"));
    }
}

void PhysBody::destroyWorld() {
    if (world) {

        NewtonDestroyAllBodies(world);
        NewtonDestroy(world);
        world = nullptr;
        DLOG("world destroyed");
    }
}

//} // namespace Physics
