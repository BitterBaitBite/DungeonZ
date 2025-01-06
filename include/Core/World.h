#pragma once

#include <cstdint>
#include <list>

#include "Gameplay/GameObject.h"

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
        // This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
        Enemy* _enemy { nullptr };

        std::list<GameObject*> _gameObjects;
};
