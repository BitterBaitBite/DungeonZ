#include "Core/DungeonManager.h"

#include "Core/WindowManager.h"
#include "Enums/DirectionEnum.h"
#include "SFML/Graphics/Rect.hpp"

DungeonManager* DungeonManager::_instance { nullptr };

DungeonManager* DungeonManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new DungeonManager();
    }

    return _instance;
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

const Room* DungeonManager::getCurrentRoom() const { return _dungeon->getCurrentRoom(); }

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
