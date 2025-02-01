#include "Core/CollisionManager.h"

#include <tmxlite/Types.hpp>

#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Enums/DirectionEnum.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/Player.h"
#include "Map/EnemyRoom.h"

CollisionManager* CollisionManager::_instance { nullptr };

CollisionManager* CollisionManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new CollisionManager();
    }

    return _instance;
}

bool CollisionManager::isOffScreen(sf::FloatRect rect) {
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();

    return
        rect.left < 0
        || rect.top < 0
        || rect.left + rect.width > windowSize.x
        || rect.top + rect.height > windowSize.y;
}

bool CollisionManager::hasObjectCollision(sf::FloatRect rect) {
    const Room* currentRoom = DungeonManager::getInstance()->getCurrentRoom();

    for (sf::FloatRect collider : currentRoom->getObjectColliders()) {
        if (rect.intersects(collider)) {
            return true;
        }
    }

    return false;
}

bool CollisionManager::hasObjectCollision(sf::FloatRect rect, std::vector<DirectionEnum>& directions) {
    const Room* currentRoom = DungeonManager::getInstance()->getCurrentRoom();
    std::vector<sf::FloatRect> colliders = currentRoom->getObjectColliders();

    bool hasCollided = false;
    for (sf::FloatRect collider : colliders) {
        if (!rect.intersects(collider)) continue;

        if (rect.left < collider.left + collider.width && rect.left + rect.width > collider.left + collider.width) {
            directions.push_back(DirectionEnum::Left);
            hasCollided = true;
        }

        if (rect.top < collider.top + collider.height && rect.top + rect.height > collider.top + collider.height) {
            directions.push_back(DirectionEnum::Up);
            hasCollided = true;
        }

        if (rect.left + rect.width > collider.left && rect.left < collider.left) {
            directions.push_back(DirectionEnum::Right);
            hasCollided = true;
        }

        if (rect.top + rect.height > collider.top && rect.top < collider.top) {
            directions.push_back(DirectionEnum::Down);
            hasCollided = true;
        }
    }

    return hasCollided;
}

// TODO
bool CollisionManager::hasPlayerCollision(sf::FloatRect rect) {
    return rect.intersects(Player::getInstance()->getCollider());
}

std::vector<Enemy*> CollisionManager::getEnemyCollisions(sf::FloatRect rect) {
    std::vector<Enemy*> collidedEnemies;

    Room* currentRoom = DungeonManager::getInstance()->getCurrentRoom();
    auto room = dynamic_cast<EnemyRoom*>(currentRoom);
    if (room == nullptr) return collidedEnemies;

    std::vector<Enemy*> activeEnemies = room->getActiveEnemies();
    for (Enemy* activeEnemy : activeEnemies) {
        if (activeEnemy == nullptr) continue;

        if (activeEnemy->getCollider().intersects(rect)) {
            collidedEnemies.push_back(activeEnemy);
        }
    }

    return collidedEnemies;
}
