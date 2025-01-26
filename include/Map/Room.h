#pragma once
#include <tmxlite/Map.hpp>

class MapLayer;

namespace sf {
    class RenderWindow;
}

class Room {
    public:
        ~Room();

        void init();
        void update(float deltaMilliseconds);
        void render(sf::RenderWindow& window);
        std::string toString(bool current = false);

    private:
        tmx::Map* _map { nullptr };
        std::vector<MapLayer*> _layers;
};
