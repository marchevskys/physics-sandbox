#ifndef GAMEAPP_H
#define GAMEAPP_H
#include <memory>

class Window;

class GameApp {
    std::unique_ptr<Window> m_window;

  public:
    GameApp();
    ~GameApp();
    void play();
    void physicsUpdate();
    void graphicsUpdate();
};

#endif // GAMEAPP_H
