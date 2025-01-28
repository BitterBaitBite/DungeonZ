#pragma once
#include <tmxlite/Map.hpp>

#include "SFML/Graphics/Rect.hpp"

class MapLayer;
class ObjectLayer;

namespace sf {
    class RenderWindow;
}

class Room {
    public:
        ~Room();

        void init();
        void update(float deltaMilliseconds);
        void render(sf::RenderWindow& window);

    private:
        tmx::Map* _map { nullptr };
        std::vector<MapLayer*> _layers;
        std::vector<ObjectLayer*> _collisionLayers;
        std::vector<sf::FloatRect> _collisionRects;

    public:
        void initObjectColliders();
        std::vector<sf::FloatRect> getObjectColliders() const { return _collisionRects; }
        std::string toString(bool current = false);
};
