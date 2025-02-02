#pragma once
#include <bitset>
#include <vector>
#include <tmxlite/Map.hpp>

#include "Enums/DirectionEnum.h"
#include "SFML/Graphics/Rect.hpp"

class MapLayer;
class ObjectLayer;

namespace sf {
    class RenderWindow;
}

class Room {
    public:
        virtual ~Room();

        virtual void init(std::vector<DirectionEnum> adjacentRooms);
        std::bitset<4> getDirectionsMask(std::vector<DirectionEnum> directions);
        const char* getRoomMapPath(std::vector<DirectionEnum> availableRooms);
        virtual void update(uint32_t deltaTime);
        virtual void render(sf::RenderWindow& window);

    protected:
        tmx::Map* _map { nullptr };
        std::vector<MapLayer*> _layers;
        std::vector<ObjectLayer*> _collisionLayers;
        std::vector<sf::FloatRect> _collisionRects;

    public:
        void initObjectColliders();
        std::vector<sf::FloatRect> getObjectColliders() const { return _collisionRects; }
        std::string toString(bool current = false);
};
