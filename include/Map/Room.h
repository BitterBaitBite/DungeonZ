#pragma once
#include <Assets/SpriteSheet.h>

#include "Map/Tile.h"
#include "Utils/Constants.h"

namespace sf {
    class RenderWindow;
}

class Room {
    public:
        Room();
        Room(sf::Vector2f pos);
        void initTile(SpriteSheet* baseSprite, sf::Texture* foamTexture, Tile* layer[ROOM_ROW_SIZE][ROOM_COL_SIZE]);
        ~Room();

    private:
        sf::Vector2f _position;
        Tile* foamLayer[ROOM_ROW_SIZE][ROOM_COL_SIZE];
        Tile* groundLayer[ROOM_ROW_SIZE][ROOM_COL_SIZE];

        void initTiles();

    public:
        void update(float deltaMilliseconds);
        void render(sf::RenderWindow& window);
};
