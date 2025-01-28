#include "Core/CollisionManager.h"

#include <tmxlite/Types.hpp>

#include "Core/WindowManager.h"
#include "Enums/FaceDirection.h"

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

FaceDirection CollisionManager::getWindowCollision(sf::FloatRect rect) {
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();

    if (rect.left < 0) return FaceDirection::Left;

    if (rect.top < 0) return FaceDirection::Up;

    if (rect.left + rect.width > windowSize.x) return FaceDirection::Right;

    if (rect.top + rect.height > windowSize.y) return FaceDirection::Down;
}
