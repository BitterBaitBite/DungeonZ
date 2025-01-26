#include <Gameplay/Villager.h>
#include <SFML/Window/Keyboard.hpp>

#include "../../build/_deps/sfml-src/src/SFML/Window/Win32/CursorImpl.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "Utils/Vector.h"

bool Villager::init(const VillagerDescriptor& villagerDescriptor) {
    _speed = villagerDescriptor.speed;

    return Enemy::init(villagerDescriptor);
}

void Villager::update(float deltaMilliseconds) {
    calcTargetPosition();

    // Update final position
    _position.x += (_direction.x * _speed.x * deltaMilliseconds);
    _position.y += (_direction.y * _speed.y * deltaMilliseconds);

    Enemy::update(deltaMilliseconds);
}

void Villager::calcTargetPosition() {
    _direction = Vector::normalize(_direction);
}
