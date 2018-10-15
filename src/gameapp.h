#ifndef GAMEAPP_H
#define GAMEAPP_H
#include <memory>

// todo: make it singletone
class Window;

class GameApp {
    std::unique_ptr<Window> m_window;

  public:
    static GameApp *GetInstance() {
        static GameApp game;
        return &game;
    }

    void play();

  private:
    GameApp(const GameApp &other) = delete;
    GameApp &operator=(const GameApp &other) = delete;
    GameApp();
    ~GameApp();
};

#endif // GAMEAPP_H
