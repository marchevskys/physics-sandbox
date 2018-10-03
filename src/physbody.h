#ifndef PHYSBODY_H
#define PHYSBODY_H
#include <array>
#include <initializer_list>
#include <type_traits>

#define _NEWTON_USE_DOUBLE
// it does not work with single precision now
#ifdef _NEWTON_USE_DOUBLE
#define dFloat double
#else
#define dFloat float
#endif

//namespace Physics {
class NewtonWorld;
class NewtonCollision;
class NewtonBody;

class CollisionShape {
  public:
    CollisionShape();
    ~CollisionShape();

  protected:
    NewtonCollision *m_collision;
    friend class PhysBody;
};

class CollisionSphere : public CollisionShape {
  public:
    CollisionSphere(dFloat radius);
    friend class NewtonBody;
};

class CollisionCapsule : public CollisionShape {
  public:
    CollisionCapsule(dFloat radius, dFloat length);
};

class CollisionCuboid : public CollisionShape {
  public:
    CollisionCuboid(dFloat w, dFloat l, dFloat h);
};

class PhysBody {
  public:
    //PhysBody();
    //PhysBody(CollisionShape shape, dFloat massMatrix[4]);
    PhysBody(CollisionShape &&shape, std::array<dFloat, 4> massMatrix = {0, 0, 0, 0});

    static std::array<dFloat, 4> momentSphere(dFloat mass, dFloat radius, dFloat shellNess = 0.0);
    static std::array<dFloat, 4> momentCuboid(dFloat mass, dFloat w, dFloat l, dFloat h, dFloat shellNess = 0.0);
    std::array<dFloat, 4> getMassAndInertiaMoment() const;

    void setForce(std::array<dFloat, 3> force);
    void getMatrix(std::array<dFloat, 16> values);
    void getMatrix(std::array<float, 16> values);
    void getMatrix(double *values);
    void getMatrix(float *values);
    void getPosition(std::array<dFloat, 3> values);
    void getVelocity(std::array<dFloat, 3> velocity);

    static void createWorld();
    static void destroyWorld();
    virtual ~PhysBody();
    static void updatePhysics(const dFloat deltaTime);
    struct Data {
        dFloat force[3]{0};
        dFloat torque[3]{0};
    } data;

    static std::array<dFloat, 3> origin;

  protected:
    NewtonBody *m_body = nullptr;
    void turnOffDefaultResistance();
    static void setForcesAndTorques(const NewtonBody *const _body, dFloat _timestep, int _threadIndex);
    //static NewtonWorld *world;

  private:
    friend class CollisionShape;
};

//} // namespace Physics
#endif // PHYSBODY_H
