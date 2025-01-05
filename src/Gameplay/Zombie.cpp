#include <Gameplay/Zombie.h>
#include <SFML/Window/Keyboard.hpp>

#include "Utils/Vector.h"

bool Zombie::init(const ZombieDescriptor& zombieDescriptor) {
    _speed = zombieDescriptor.speed;

    return Enemy::init(zombieDescriptor);
}

void Zombie::update(float deltaMilliseconds) {
    calcMoveInput();

    // Update final position
    _position.x += (_direction.x * _speed.x * deltaMilliseconds);
    _position.y += (_direction.y * _speed.y * deltaMilliseconds);

    Enemy::update(deltaMilliseconds);
}

void Zombie::calcMoveInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        _direction.x = -1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        _direction.x = 1.0f;
    }
    else {
        _direction.x = .0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        _direction.y = -1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        _direction.y = 1.0f;
    }
    else {
        _direction.y = .0f;
    }

    _direction = normalize(_direction);
}
