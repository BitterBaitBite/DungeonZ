#pragma once

#include "Enums/Difficulty.h"
#include "Map/Dungeon.h"
#include "SFML/Graphics/Rect.hpp"

class MainMenu;

class DungeonManager {
    public:
        static DungeonManager* getInstance();

        ~DungeonManager();

    private:
        static DungeonManager* _instance;
        Dungeon* _dungeon { nullptr };
        Difficulty _difficulty { Difficulty::Easy };

        void debugCollisionGrid(std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& array) const;

    public:
        Dungeon* loadDungeon();

        Difficulty getDifficulty() const { return _difficulty; }
        void setDifficulty(Difficulty difficulty) { _difficulty = difficulty; }

        const sf::Vector2i getRoomPosition() const;
        Room* getCurrentRoom() const;

        bool moveRoom(DirectionEnum direction);
        bool moveRoom(sf::FloatRect collider);

        sf::Vector2i getEnemyCount() const { return _dungeon->getEnemyCount(); }
        std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE> getRoomCostGrid();
};
