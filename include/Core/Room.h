#pragma once
#include "Tile.h"

class Room {
    public:
        struct RoomDescriptor {
            int rows;
            int cols;
        };

        Room(int rows, int cols);

        void init(RoomDescriptor roomDescriptor);

        // sf::Vector2<Tile> roomTiles;
};
