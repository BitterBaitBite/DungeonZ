#include <Gameplay/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Enums/DirectionEnum.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Constants.h"
#include "Utils/Random.h"
#include "Utils/Vector.h"

bool Enemy::init(const EnemyDescriptor& enemyDescriptor) {
    float posX = enemyDescriptor.position.x - enemyDescriptor.spriteWidth / 2;
    float posY = enemyDescriptor.position.y - enemyDescriptor.spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));
    _speed = enemyDescriptor.speed;


    _sprite.setScale(enemyDescriptor.scale);
    _sprite.setTexture(*enemyDescriptor.texture);
    _spriteWidth = enemyDescriptor.spriteWidth;
    _spriteHeight = enemyDescriptor.spriteHeight;
    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));

    _maxHealth = enemyDescriptor.maxHealth;
    _currentHealth = _maxHealth;

    _collisionBounds = {
        posX,
        posY,
        TILE_WIDTH,
        TILE_HEIGHT
    };

    return true;
}

void Enemy::update(float deltaMilliseconds) {
    if (_isDead) return;

    _time += deltaMilliseconds;

    // checkAttackConditions();
    if (_isAttacking) {
        attack(deltaMilliseconds);
    }

    if (!_isAttacking) {
        move(deltaMilliseconds);
    }
}

void Enemy::render(sf::RenderWindow& window) {
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

void Enemy::debugSprite(sf::RenderWindow& window) {
    // Global bounds
    const sf::FloatRect spriteBounds = _sprite.getGlobalBounds();
    sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
    boundsRect.setOutlineColor(sf::Color::Yellow);
    boundsRect.setOutlineThickness(2.f);
    boundsRect.setFillColor(sf::Color::Transparent);

    // Collision bounds
    sf::RectangleShape collisionRect(sf::Vector2f(_collisionBounds.width, _collisionBounds.height));

    collisionRect.setPosition(_collisionBounds.left, _collisionBounds.top);
    collisionRect.setOutlineColor(sf::Color::Red);
    collisionRect.setOutlineThickness(2.f);
    collisionRect.setFillColor(sf::Color::Transparent);

    // Attack collider
    // sf::RectangleShape attackRect(sf::Vector2f(_attackCollider.width, _attackCollider.height));
    //
    // attackRect.setPosition(_attackCollider.left, _attackCollider.top);
    // attackRect.setOutlineColor(sf::Color::Magenta);
    // attackRect.setOutlineThickness(2.f);
    // attackRect.setFillColor(sf::Color::Transparent);

    window.draw(boundsRect);
    window.draw(collisionRect);
    // window.draw(attackRect);
}

void Enemy::setPosition(const sf::Vector2f& newPosition) {
    GameObject::setPosition(newPosition);
    _sprite.setPosition(newPosition);
}


void Enemy::move(float deltaMilliseconds) {
    getMoveDirection();
    setFacingDirection();
    setMoveAnimation();
    setMovePosition(deltaMilliseconds);
}

void Enemy::getMoveDirection() {
    int seconds = _time / 1000;
    bool isPatrolling = false;

    _direction = { 0, 0 };
    auto direction = static_cast<DirectionEnum>(Random::randomInt(0, 3));

    if (seconds % 3 == 0) {
        isPatrolling = !isPatrolling;
    }

    if (isPatrolling) {
        if (direction == DirectionEnum::Left) {
            _direction.x = -1.0f;
        }
        if (direction == DirectionEnum::Right) {
            _direction.x = 1.0f;
        }

        if (direction == DirectionEnum::Up) {
            _direction.y = -1.0f;
        }
        if (direction == DirectionEnum::Down) {
            _direction.y = 1.0f;
        }
    }

    _direction = Vector::normalize(_direction);

    float magnitude = Vector::magnitude(_direction);
    magnitude > 0.0f ? _isMoving = true : _isMoving = false;
}

void Enemy::setFacingDirection() {
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

void Enemy::setMoveAnimation() {
    _currentTile.x = FRAMES_PER_SECOND * _time / 1000.0f;
    _currentTile.x %= MAX_MOVEMENT_TILES;

    if (_isMoving) {
        _currentTile.y = RUN_ROW;
    }
    else {
        _currentTile.y = IDLE_ROW;
    }
}

void Enemy::setMovePosition(float deltaMilliseconds) {
    auto velocity = sf::Vector2f(_direction.x * _speed.x * deltaMilliseconds,
                                 _direction.y * _speed.y * deltaMilliseconds);

    // Check window bounds collisions
    sf::FloatRect nextCollisionBounds;
    getNextCollisionBounds(velocity, nextCollisionBounds);

    if (CollisionManager::getInstance()->isOffScreen(nextCollisionBounds)) {
        return;
    }

    std::vector<DirectionEnum> collisionDirections;
    if (CollisionManager::getInstance()->hasObjectCollision(nextCollisionBounds, collisionDirections)) {
        return;
    }

    // Update player position
    _position.x += velocity.x;
    _position.y += velocity.y;

    // Update collider position
    _collisionBounds.left += velocity.x;
    _collisionBounds.top += velocity.y;

    _sprite.setPosition(_position);
}

void Enemy::attack(float deltaMilliseconds) {}

void Enemy::attackAnimation() {}

void Enemy::getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds) {
    nextBounds = _collisionBounds;
    nextBounds.left += velocity.x;
    nextBounds.top += velocity.y;
}

void Enemy::addHealth(int amount) {
    _currentHealth = std::clamp(_currentHealth + amount, 0, _maxHealth);
}
