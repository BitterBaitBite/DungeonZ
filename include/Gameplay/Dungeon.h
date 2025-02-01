#pragma once

#include "Map/Room.h"
#include "SFML/System/Vector2.hpp"
#include "Utils/Constants.h"

enum class DirectionEnum;

namespace sf {
    class RenderWindow;
}

class Dungeon {
    public:
        Dungeon();
        ~Dungeon();

        Room* getCurrentRoom() const {
            if (_currentRoom.x < 0
                || _currentRoom.y < 0
                || _currentRoom.x >= DUNGEON_ROOMS
                || _currentRoom.y >= DUNGEON_ROOMS
            ) {
                return nullptr;
            }

            return _rooms[_currentRoom.x][_currentRoom.y];
        }

        const sf::Vector2i getRoomPosition() const { return _currentRoom; }

        void init();
        void render(sf::RenderWindow& window);
        void update(float deltaMilliseconds);

        bool HasRoom(int row, int col) const;
        bool HasAdjacentRoom(int row, int col);
        bool HasAdjacentRoom(DirectionEnum direction) const;
        bool HasAdjacentRoom(int row, int col, DirectionEnum direction) const;
        std::vector<DirectionEnum> GetAdjacentRooms(int row, int col) const;
        bool moveTo(DirectionEnum direction);

    private:
        sf::Vector2i _currentRoom { 0, 0 };
        Room* _rooms[DUNGEON_ROOMS][DUNGEON_ROOMS];

        void DebugDungeon();
        void GenerateRooms();
        void InitializeRooms();
};
