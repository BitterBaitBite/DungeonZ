#pragma once

#include <tmxlite/FreeFuncs.hpp>
#include <tmxlite/Map.hpp>

#include "Gameplay/Dungeon.h"
#include "Gameplay/Player.h"

class Background;
class Room;
class Enemy;

namespace sf {
    class RenderWindow;
}

class MapLayer;

class World {
    public:
        ~World();

        bool load();
        void update(uint32_t deltaMilliseconds);
        void render(sf::RenderWindow& window);

    private:
        Player* _player { nullptr };
        Dungeon* _currentDungeon { nullptr }; // Dungeon = Level
};
