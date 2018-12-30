#ifndef PHYSBODY_H
#define PHYSBODY_H
#include <vector>

// PHYSICS WORLD
class PhysWorld {
    class NewtonWorld *m_world;

  public:
    PhysWorld();
    ~PhysWorld();
    inline const NewtonWorld *get() const { return m_world; }
    void update(const double dt);
};

// COLLISION SHAPE
class CollisionShape {
  protected:
    class NewtonCollision *m_collision = nullptr;

  public:
    CollisionShape(const PhysWorld &world);
    virtual ~CollisionShape() = 0;
    const NewtonCollision *get() { return m_collision; }
};

class CollisionSphere : public CollisionShape {
  public:
    CollisionSphere(const PhysWorld &world, double radius = 1.0);
};

class CollisionIcosahedron : public CollisionShape {
  public:
    CollisionIcosahedron(const PhysWorld &world, double radius = 1.0);
};

class CollisionCuboid : public CollisionShape {
  public:
    CollisionCuboid(const PhysWorld &world, double w, double l, double h);
};

class CollisionConvex : public CollisionShape {
  public:
    CollisionConvex(const PhysWorld &world, const std::vector<double> points);
};

// PHYSICS BODY
class PhysBody {
  public:
    // zero mass by default (body is static)
    PhysBody(){};
    PhysBody(const PhysWorld &world, CollisionShape &&shape, const double m = 0.0, const double Ixx = 0.0, const double Iyy = 0.0, const double Izz = 0.0);
    PhysBody(const PhysBody &other);
    PhysBody(PhysBody &&other) noexcept;

    void setPos(const double *pos);
    void getPos(double *pos);

    void setVelocity(const double *velocity);
    void addForce(const double *force);
    void addForce(const double x, const double y, const double z);
    void addTorque(const double *torque);
    void addTorque(const double x, const double y, const double z);
    void setOmega(const double *omega);
    void setMassMatrix(const double *mass);
    void getMatrix(double *mat) const;

    struct Data {
        double force[3]{0};
        double torque[3]{0};
    } data;

    ~PhysBody();

    static void setOrigin(double originX, double originY, double originZ);

  protected:
    class NewtonBody *m_body = nullptr;

  private:
    void turnOffDefaultResistance();
};

#endif // PHYSBODY_H
