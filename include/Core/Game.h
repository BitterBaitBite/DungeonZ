#pragma once

#include <cstdint>
#include <string>

namespace sf {
    class RenderWindow;
}

class World;


class Game {
    public:
        struct GameCreateInfo {
            std::string gameTitle;
            uint32_t screenWidth;
            uint32_t screenHeight;
            uint32_t frameRateLimit;
        };

        ~Game();

        bool init();

        bool isRunning() const;

        void update(uint32_t deltaMilliseconds);
        void render();

    private:
        sf::RenderWindow* _window { nullptr };
        World* _world { nullptr };
};
