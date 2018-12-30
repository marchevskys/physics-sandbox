#ifndef OBJECT_H
#define OBJECT_H
#include <physbody.h>
#include <model.h>
#include <string>

#include <memory>


class Object
{
    std::string m_name;
    std::unique_ptr<PhysBody> m_body;
    std::shared_ptr<Model> m_model;
public:
    Object(PhysBody* body = nullptr, Model* model = nullptr){
        if(!body)
        {
        }
          m_body.reset(body);
          m_model.reset(model);
    };
};

class ObjectCreator{
    const PhysWorld& m_world;
    ObjectCreator(const PhysWorld& world) : m_world(world){};
    Object createSphere(double radius){
        Object sphere(new PhysBody(m_world, CollisionSphere(m_world, radius),1.0, 1.0, 1.0, 1.0));

    }
};

#endif // OBJECT_H
