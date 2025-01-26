#pragma once

#include <unordered_set>

#include "Gameplay/GameObject.h"
#include "Gameplay/Player.h"

class Background;
class Room;
class Enemy;

namespace sf {
    class RenderWindow;
}

class World {
    public:
        ~World();

        // TO-DO: Ideally the scene should be read from file.
        bool load(sf::RenderWindow* window);

        void update(uint32_t deltaMilliseconds);
        void render(sf::RenderWindow& window);

    private:
        Background* _background { nullptr };
        Room* _level01 { nullptr };
        std::unordered_set<Enemy*> _enemyList { nullptr };
        Player* _player { nullptr };
};
