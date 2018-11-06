#ifndef CAROBJECT_H
#define CAROBJECT_H
#include "declarations.h"

#include <memory>
#include <vector>

class CarObject {
    //    std::unique_ptr<GameObject> m_carBody;
    GameObject *go;

  public:
    CarObject();
};

#endif // CAROBJECT_H
