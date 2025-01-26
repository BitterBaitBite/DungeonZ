#include <Gameplay/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Utils/Constants.h"
#include "Utils/Vector.h"

void Player::setPosition(const sf::Vector2f& newPosition) {
    _position = newPosition;
    _sprite.setPosition(newPosition);
}

bool Player::init(const PlayerDescriptor& playerDescriptor) {
    float posX = playerDescriptor.position.x - playerDescriptor.spriteWidth / 2;
    float posY = playerDescriptor.position.y - playerDescriptor.spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));

    _sprite.setScale(playerDescriptor.scale);
    _sprite.setTexture(*playerDescriptor.texture);
    _speed = playerDescriptor.speed;

    _spriteWidth = playerDescriptor.spriteWidth;
    _spriteHeight = playerDescriptor.spriteHeight;
    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));

    return true;
}

void Player::update(float deltaMilliseconds) {
    _time += deltaMilliseconds;

    if (_isDead) return;

    getAttackInput();
    if (_isAttacking) {
        attack(deltaMilliseconds);
    }

    if (!_isAttacking) {
        move(deltaMilliseconds);
    }
}

void Player::render(sf::RenderWindow& window) {
    sf::IntRect spriteRect = _sprite.getTextureRect();
    sf::Vector2f tilePosition { _currentTile.x * _spriteWidth, _currentTile.y * _spriteHeight };

    switch (_faceDirectionX) {
        case FaceDirectionX::Left:
            spriteRect =
                sf::IntRect(tilePosition.x + _spriteWidth, tilePosition.y, -_spriteWidth, _spriteHeight);
            break;

        case FaceDirectionX::Right:
            spriteRect =
                sf::IntRect(tilePosition.x, tilePosition.y, _spriteWidth, _spriteHeight);
            break;

        default:
            break;
    }

    _sprite.setTextureRect(spriteRect);
    window.draw(_sprite);

    debugSprite(window);
}

void Player::debugSprite(sf::RenderWindow& window) {
    const sf::FloatRect spriteBounds = _sprite.getGlobalBounds();
    sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
    boundsRect.setOutlineColor(sf::Color::Cyan);
    boundsRect.setOutlineThickness(2.f);
    boundsRect.setFillColor(sf::Color::Transparent);

    window.draw(boundsRect);
}

/* PLAYER ATTACK */
void Player::attack(float deltaMilliseconds) {
    attackAnimation();
    _attackTime += deltaMilliseconds;
}

void Player::resetAttackCounter() { _attackCounter = 0; }

void Player::resetAttackAnimation() {
    _currentTile.x = 0;
    _attackTime = 0.0f;
}

void Player::getAttackInput() {
    if (_isAttacking) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        _isAttacking = true;
        _comboDelayTime = _time;

        if (++_attackCounter >= MAX_ATTACK_COUNT) resetAttackCounter();

        resetAttackAnimation();
    }
    else if (_comboDelayTime > 0 && _time - _comboDelayTime > MAX_COMBO_DELAY * 1000.0f) {
        resetAttackCounter();
        resetAttackAnimation();
        _comboDelayTime = 0.0f;
    }
}

void Player::attackAnimation() {
    _currentTile.x = FRAMES_PER_SECOND * _attackTime / 1000.0f;

    if (_currentTile.x >= 6) {
        _currentTile.x %= 6;
        _isAttacking = false;
    }

    int tileColumn = DEFAULT_TILE_ROW + _attackCounter + 2 * static_cast<int>(_faceDirection);
    if (_faceDirection == FaceDirection::Left) tileColumn = DEFAULT_TILE_ROW + _attackCounter;

    _currentTile.y = tileColumn;
}


/* PLAYER MOVEMENT */
void Player::move(float deltaMilliseconds) {
    getMoveInput();
    setFacingDirection();
    setMoveAnimation();
    setPosition(deltaMilliseconds);
}

void Player::getMoveInput() {
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

    float magnitude = Vector::magnitude(_direction);
    magnitude > 0.0f ? _isMoving = true : _isMoving = false;
}

void Player::setFacingDirection() {
    _lastFaceDirection = _faceDirection;

    if (_direction.x > 0.0f) {
        _faceDirection = FaceDirection::Right;
        _faceDirectionX = FaceDirectionX::Right;
    }
    else if (_direction.x < 0.0f) {
        _faceDirection = FaceDirection::Left;
        _faceDirectionX = FaceDirectionX::Left;
    }
    else if (_direction.y > 0.0f) {
        _faceDirection = FaceDirection::Down;
        _faceDirectionY = FaceDirectionY::Down;
    }
    else if (_direction.y < 0.0f) {
        _faceDirection = FaceDirection::Up;
        _faceDirectionY = FaceDirectionY::Up;
    }
}

void Player::setMoveAnimation() {
    _currentTile.x = FRAMES_PER_SECOND * _time / 1000.0f;
    _currentTile.x %= MAX_TILE_FRAMES;

    if (_isMoving) {
        _currentTile.y = 1;
    }
    else {
        _currentTile.y = 0;
    }
}

void Player::setPosition(float deltaMilliseconds) {
    // Update final position
    _position.x += (_direction.x * _speed.x * deltaMilliseconds);
    _position.y += (_direction.y * _speed.y * deltaMilliseconds);

    _sprite.setPosition(_position);
}
