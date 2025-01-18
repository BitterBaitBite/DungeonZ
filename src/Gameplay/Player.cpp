#include <Gameplay/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "SFML/Window/Keyboard.hpp"
#include "Utils/Vector.h"

bool Player::init(const PlayerDescriptor& playerDescriptor) {
    setPosition(playerDescriptor.position);

    _sprite.setScale(playerDescriptor.scale);
    _sprite.setTexture(*playerDescriptor.texture);
    _speed = playerDescriptor.speed;
    _tileWidth = playerDescriptor.tileWidth;
    _tileHeight = playerDescriptor.tileHeight;

    return true;
}

void Player::update(float deltaMilliseconds) {
    move(deltaMilliseconds);
}

void Player::render(sf::RenderWindow& window) {
    // Extend this mechanism to be able to support animations
    _sprite.setTextureRect(sf::IntRect(0, 0, _tileWidth, _tileHeight));

    window.draw(_sprite);
}

void Player::move(float deltaMilliseconds) {
    calcMoveInput();

    // Update final position
    _position.x += (_direction.x * _speed.x * deltaMilliseconds);
    _position.y += (_direction.y * _speed.y * deltaMilliseconds);

    _sprite.setPosition(_position);
}


void Player::calcMoveInput() {
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

void Player::calcFacingDirection() {
    float magnitude = Vector::magnitude(_direction);
    if (magnitude > 0.0f) {
        _isMoving = true;

        if (_direction.x > 0.0f && _direction.y > 0.0f) {
            if (_direction.x > _direction.y) {
                _faceDirection = FaceDirection::Right;
            }
            else {
                _faceDirection = FaceDirection::Up;
            }
        }
        else if (_direction.x > 0.0f && _direction.y < 0.0f) {
            if (_direction.x > -_direction.y) {
                _faceDirection = FaceDirection::Right;
            }
            else {
                _faceDirection = FaceDirection::Down;
            }
        }
        else if (_direction.x < 0.0f && _direction.y > 0.0f) {
            if (-_direction.x > _direction.y) {
                _faceDirection = FaceDirection::Left;
            }
            else {
                _faceDirection = FaceDirection::Up;
            }
        }
        else if (_direction.x < 0.0f && _direction.y < 0.0f) {
            if (-_direction.x > -_direction.y) {
                _faceDirection = FaceDirection::Left;
            }
            else {
                _faceDirection = FaceDirection::Down;
            }
        }
    }
}
