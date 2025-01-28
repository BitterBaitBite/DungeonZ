#include <Gameplay/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Utils/Constants.h"
#include "Utils/Vector.h"

void Player::setPosition(const sf::Vector2f& newPosition) {
    GameObject::setPosition(newPosition);
    _sprite.setPosition(newPosition);
}

bool Player::init(const PlayerDescriptor& playerDescriptor) {
    float posX = playerDescriptor.position.x - playerDescriptor.spriteWidth / 2;
    float posY = playerDescriptor.position.y - playerDescriptor.spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));
    _speed = playerDescriptor.speed;


    _sprite.setTexture(*playerDescriptor.texture);
    _sprite.setScale(playerDescriptor.scale);
    _spriteWidth = playerDescriptor.spriteWidth;
    _spriteHeight = playerDescriptor.spriteHeight;
    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));

    _maxHealth = playerDescriptor.maxHealth;
    _currentHealth = _maxHealth;

    _collisionBounds = {
        playerDescriptor.position.x - TILE_WIDTH / 3,
        playerDescriptor.position.y,
        2 * TILE_WIDTH / 3,
        TILE_HEIGHT / 2
    };

    return true;
}

void Player::update(float deltaMilliseconds) {
    if (_isDead) return;

    _time += deltaMilliseconds;

    getAttackInput();
    if (_isAttacking) {
        attack(deltaMilliseconds);
    }

    if (!_isAttacking) {
        move(deltaMilliseconds);
    }
}

void Player::render(sf::RenderWindow& window) {
    // TODO: render dead sprite
    if (_isDead) return;

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
    // Global bounds
    const sf::FloatRect spriteBounds = _sprite.getGlobalBounds();
    sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
    boundsRect.setOutlineColor(sf::Color::Cyan);
    boundsRect.setOutlineThickness(2.f);
    boundsRect.setFillColor(sf::Color::Transparent);

    // Collision bounds
    sf::RectangleShape collisionRect(sf::Vector2f(_collisionBounds.width, _collisionBounds.height));

    collisionRect.setPosition(_collisionBounds.left, _collisionBounds.top);
    collisionRect.setOutlineColor(sf::Color::Green);
    collisionRect.setOutlineThickness(2.f);
    collisionRect.setFillColor(sf::Color::Transparent);

    // Attack collider
    sf::RectangleShape attackRect(sf::Vector2f(_attackCollider.width, _attackCollider.height));

    attackRect.setPosition(_attackCollider.left, _attackCollider.top);
    attackRect.setOutlineColor(sf::Color::Magenta);
    attackRect.setOutlineThickness(2.f);
    attackRect.setFillColor(sf::Color::Transparent);

    window.draw(boundsRect);
    window.draw(collisionRect);
    window.draw(attackRect);
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

    _attackCollider = { 0.f, 0.f, 0.f, 0.f };
}

void Player::attackAnimation() {
    _currentTile.x = FRAMES_PER_SECOND * _attackTime / 1000.0f;

    if (_currentTile.x == 2) {
        setAttackCollider();
    }

    if (_currentTile.x >= 6) {
        _currentTile.x %= 6;
        _isAttacking = false;
    }

    int tileColumn = DEFAULT_TILE_ROW + _attackCounter + 2 * static_cast<int>(_faceDirection);
    if (_faceDirection == DirectionEnum::Left) tileColumn = DEFAULT_TILE_ROW + _attackCounter;

    _currentTile.y = tileColumn;
}

void Player::setAttackCollider() {
    sf::Vector2f colliderPos = {
        _collisionBounds.left - _collisionBounds.width / 4,
        _collisionBounds.top - _collisionBounds.height
    };
    switch (_faceDirection) {
        case DirectionEnum::Right:
            colliderPos.x = _collisionBounds.left + _collisionBounds.width;
            break;

        case DirectionEnum::Left:
            colliderPos.x = _collisionBounds.left - TILE_WIDTH;
            break;

        case DirectionEnum::Up:
            colliderPos.y = _collisionBounds.top - TILE_HEIGHT;
            break;

        case DirectionEnum::Down:
            colliderPos.y = _collisionBounds.top + _collisionBounds.height;
            break;
    }

    _attackCollider = sf::FloatRect(colliderPos.x, colliderPos.y, TILE_WIDTH, TILE_HEIGHT);
}


/* PLAYER MOVEMENT */
void Player::move(float deltaMilliseconds) {
    getMoveInput();
    setFacingDirection();
    setMoveAnimation();
    setMovePosition(deltaMilliseconds);
}

void Player::getMoveInput() {
    _direction = { 0, 0 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _direction.x = -1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _direction.x = 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _direction.y = -1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _direction.y = 1.0f;
    }

    _direction = Vector::normalize(_direction);

    float magnitude = Vector::magnitude(_direction);
    magnitude > 0.0f ? _isMoving = true : _isMoving = false;
}

void Player::setFacingDirection() {
    if (_direction.x > 0.0f) {
        _faceDirection = DirectionEnum::Right;
        _faceDirectionX = FaceDirectionX::Right;
    }
    else if (_direction.x < 0.0f) {
        _faceDirection = DirectionEnum::Left;
        _faceDirectionX = FaceDirectionX::Left;
    }
    else if (_direction.y > 0.0f) {
        _faceDirection = DirectionEnum::Down;
        _faceDirectionY = FaceDirectionY::Down;
    }
    else if (_direction.y < 0.0f) {
        _faceDirection = DirectionEnum::Up;
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

void Player::getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds) {
    nextBounds = _collisionBounds;
    nextBounds.left += velocity.x;
    nextBounds.top += velocity.y;
}

void Player::setMovePosition(float deltaMilliseconds) {
    auto velocity = sf::Vector2f(_direction.x * _speed.x * deltaMilliseconds,
                                 _direction.y * _speed.y * deltaMilliseconds);

    // Check window bounds collisions
    sf::FloatRect nextCollisionBounds;
    getNextCollisionBounds(velocity, nextCollisionBounds);

    if (CollisionManager::getInstance()->isOffScreen(nextCollisionBounds)) {
        if (DungeonManager::getInstance()->moveRoom(nextCollisionBounds)) {
            WarpPosition();
        }
        else return;
    }

    std::vector<DirectionEnum> collisionDirections;
    if (CollisionManager::getInstance()->hasObjectCollision(nextCollisionBounds, collisionDirections)) {
        BouncePosition(velocity, collisionDirections);
    }

    // Update player position
    _position.x += velocity.x;
    _position.y += velocity.y;

    // Update collider position
    _collisionBounds.left += velocity.x;
    _collisionBounds.top += velocity.y;

    _sprite.setPosition(_position);
}

void Player::WarpPosition() {
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();
    switch (_faceDirection) {
        case DirectionEnum::Right:
            _position.x = _position.x - windowSize.x + _collisionBounds.width + 5; // CONST WARP_OFFSET
            _collisionBounds.left = _collisionBounds.left - windowSize.x + _collisionBounds.width + 5;
            break;

        case DirectionEnum::Left:
            _position.x = _position.x + windowSize.x - _collisionBounds.width - 5;
            _collisionBounds.left = _collisionBounds.left + windowSize.x - _collisionBounds.width - 5;
            break;

        case DirectionEnum::Up:
            _position.y = _position.y + windowSize.y - _collisionBounds.height - 5;
            _collisionBounds.top = _collisionBounds.top + windowSize.y - _collisionBounds.height - 5;
            break;

        case DirectionEnum::Down:
            _position.y = _position.y - windowSize.y + _collisionBounds.height + 5;
            _collisionBounds.top = _collisionBounds.top - windowSize.y + _collisionBounds.height + 5;
            break;
    }
}

void Player::BouncePosition(sf::Vector2f velocity) {
    // Negate player position
    _position.x -= velocity.x;
    _position.y -= velocity.y;

    // Negate collider position
    _collisionBounds.left -= velocity.x;
    _collisionBounds.top -= velocity.y;
}

void Player::BouncePosition(sf::Vector2f& velocity, std::vector<DirectionEnum> directions) {
    for (DirectionEnum direction : directions) {
        switch (direction) {
            case DirectionEnum::Right:
            case DirectionEnum::Left:
                // _position.x -= velocity.x;
                // _collisionBounds.left -= velocity.x;
                // break;
                velocity.x = 0;
                break;

            case DirectionEnum::Up:
            case DirectionEnum::Down:
                // _position.y -= velocity.y;
                // _collisionBounds.top -= velocity.y;
                // break;
                velocity.y = 0;
                break;
        }
    }
}

void Player::addHealth(int amount) {
    _currentHealth = std::clamp(_currentHealth + amount, 0, _maxHealth);
}
