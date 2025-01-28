#pragma once

#include "Map/Room.h"
#include "SFML/System/Vector2.hpp"
#include "Utils/Constants.h"

enum class FaceDirection;

namespace sf {
    class RenderWindow;
}

class Dungeon {
    public:
        Dungeon();
        ~Dungeon();

        const sf::Vector2i getCurrentRoom() const { return _currentRoom; }
        void setCurrentRoom(int row, int col);

        void init();
        void render(sf::RenderWindow& window);
        void update(float deltaMilliseconds);

        bool moveTo(FaceDirection direction);
        bool HasAdjacentRoom(FaceDirection direction) const;
        bool HasAdjacentRoom(int row, int col);
        bool HasRoom(int row, int col) const;

    private:
        sf::Vector2i _currentRoom { 0, 0 };
        Room* _rooms[DUNGEON_ROOMS][DUNGEON_ROOMS];

        void DebugDungeon();
        void GenerateRooms();
        void InitializeRooms();
};
