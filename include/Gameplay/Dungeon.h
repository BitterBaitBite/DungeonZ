#pragma once

#include "Map/Room.h"
#include "Utils/Constants.h"

namespace sf {
    class RenderWindow;
}

class Dungeon {
    public:
        void InitializeNull();
        Dungeon();
        ~Dungeon();

        Room* getRoom(int row, int col) const;

        void render(sf::RenderWindow& window);
        void update(float deltaMilliseconds);

    private:
        Room* _rooms[DUNGEON_ROOMS][DUNGEON_ROOMS];

        void DebugDungeon();
        void GenerateRooms();
        void InitializeRooms();

        bool HasAdjacentRoom(int row, int col);
};
