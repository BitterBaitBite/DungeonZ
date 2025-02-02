#include "Core/DungeonManager.h"

#include "Core/WindowManager.h"
#include "Enums/DirectionEnum.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/Rect.hpp"

DungeonManager* DungeonManager::_instance { nullptr };

DungeonManager* DungeonManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new DungeonManager();
    }

    return _instance;
}

DungeonManager::~DungeonManager() {
    delete _dungeon;
    delete _instance;
}

Dungeon* DungeonManager::loadDungeon() {
    if (_dungeon == nullptr) {
        _dungeon = new Dungeon();
    }

    return _dungeon;
}

const sf::Vector2i DungeonManager::getRoomPosition() const {
    if (!_dungeon || _dungeon == nullptr) {
        return { 0, 0 };
    }

    return _dungeon->getRoomPosition();
}

Room* DungeonManager::getCurrentRoom() const { return _dungeon->getCurrentRoom(); }

bool DungeonManager::moveRoom(DirectionEnum direction) {
    if (!_dungeon->HasAdjacentRoom(direction)) return false;

    return _dungeon->moveTo(direction);
}

bool DungeonManager::moveRoom(sf::FloatRect collider) {
    sf::Vector2i currentRoom = getRoomPosition();
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();

    // Move left
    if (collider.left < 0 && _dungeon->HasRoom(currentRoom.x, currentRoom.y - 1)) {
        return _dungeon->moveTo(DirectionEnum::Left);
    }

    // Move right
    if (collider.left + collider.width > windowSize.x && _dungeon->HasRoom(currentRoom.x, currentRoom.y + 1)) {
        return _dungeon->moveTo(DirectionEnum::Right);
    }

    // Move up
    if (collider.top < 0 && _dungeon->HasRoom(currentRoom.x - 1, currentRoom.y)) {
        return _dungeon->moveTo(DirectionEnum::Up);
    }

    // Move down
    if (collider.top + collider.height > windowSize.y && _dungeon->HasRoom(currentRoom.x + 1, currentRoom.y)) {
        return _dungeon->moveTo(DirectionEnum::Down);
    }

    return false;
}

std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE> DungeonManager::getRoomCostGrid() {
    std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE> collisionGrid;

    std::vector<sf::FloatRect> roomCollisions = _dungeon->getCurrentRoom()->getObjectColliders();

    for (int i = 0; i < BACKGROUND_COL_SIZE; i++) {
        for (int j = 0; j < BACKGROUND_ROW_SIZE; j++) {
            collisionGrid[i][j] = false;

            for (sf::FloatRect& collider : roomCollisions) {
                if (collider.contains(j * TILE_WIDTH + TILE_WIDTH / 2, i * TILE_HEIGHT + TILE_HEIGHT / 2)) {
                    collisionGrid[i][j] = true;
                    break;
                }
            }
        }
    }

    // debugCollisionGrid(collisionGrid);

    return collisionGrid;
}

void DungeonManager::debugCollisionGrid(
    std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& collisionGrid) const {
    std::string debugCollisionGrid = "--------------------------------------------\n";
    for (int i = 0; i < BACKGROUND_COL_SIZE; i++) {
        debugCollisionGrid += "| ";
        for (int j = 0; j < BACKGROUND_ROW_SIZE; j++) {
            debugCollisionGrid += " [";
            debugCollisionGrid += (collisionGrid[i][j] ? "1" : "0");
            debugCollisionGrid += "] ";
        }
        debugCollisionGrid += " |\n";
    }
    debugCollisionGrid += "--------------------------------------------\n";
    printf("%s\n", debugCollisionGrid.c_str());
}
