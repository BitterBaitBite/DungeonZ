#include <Gameplay/Zombie.h>
#include <SFML/Window/Keyboard.hpp>

#include "../../build/_deps/sfml-src/src/SFML/Window/Win32/CursorImpl.hpp"
#include "SFML/Window/ContextSettings.hpp"
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
    printf("x: %f, y: %f \n", _position.x, _position.y);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _direction.x = -1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _direction.x = 1.0f;
    }
    else {
        _direction.x = .0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _direction.y = -1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _direction.y = 1.0f;
    }
    else {
        _direction.y = .0f;
    }

    _direction = Vector::normalize(_direction);
}
