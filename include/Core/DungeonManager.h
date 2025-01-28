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

        sf::Vector2i getCurrentRoom() const;
        bool moveRoom(FaceDirection direction);
        bool moveRoom(sf::FloatRect collider);
};
