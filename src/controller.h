#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller
{
public:
    Controller();
    virtual ~Controller();
};

class InputController : public Controller
{
    enum class Key { W, A, S, D };

    bool key[1024];
  public:
    void update(double dt){};
};

class AIController : public Controller
{
  public:

};

#endif // CONTROLLER_H
