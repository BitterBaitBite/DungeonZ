#pragma once
#include "Gameplay/Dungeon.h"
#include "SFML/Graphics/Rect.hpp"

class DungeonManager {
    public:
        static DungeonManager* getInstance();

        ~DungeonManager() = default;

    private:
        static DungeonManager* _instance;
        Dungeon* _dungeon { nullptr };

    public:
        Dungeon* loadDungeon();

        const sf::Vector2i getRoomPosition() const;
        const Room* getCurrentRoom() const;

        bool moveRoom(DirectionEnum direction);
        bool moveRoom(sf::FloatRect collider);
};
