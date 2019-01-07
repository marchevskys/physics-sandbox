#ifndef CAR_H
#define CAR_H

#include "physbody.h"
#include "model.h"

#include <list>
#include <memory>

template <class T>
using sptr = std::shared_ptr<T>;
template <class T>
using uptr = std::unique_ptr<T>;


class Car
{
public:
    Car(const PhysWorld* world);
    PhysBody chassis;
    Model carModel;
    std::list<std::pair<PhysBody, Model>> wheels;
};

#endif // CAR_H
