#ifndef GAMEAPP_H
#define GAMEAPP_H
#include <memory>

// todo: make it singletone
class Window;

class GameApp {
    std::unique_ptr<Window> m_window;

  public:
    GameApp();
    ~GameApp();
    void play();
};

#endif // GAMEAPP_H
