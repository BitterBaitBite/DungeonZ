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

        // TO-DO: Ideally the scene should be read from file.
        bool load(sf::RenderWindow* window);

        void update(uint32_t deltaMilliseconds);
        void render(sf::RenderWindow& window);

    private:
        Player* _player { nullptr };
        Dungeon* _currentDungeon { nullptr };
        std::vector<Enemy*> _enemyList;

        // tmxlite
        // To-Do: This should be in its own class, something like "Level" should work
        // tmx::Map* _map { nullptr };
        // std::vector<MapLayer*> _mapLayers;
};
