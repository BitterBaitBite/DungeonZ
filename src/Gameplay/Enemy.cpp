#include <Gameplay/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Enums/DirectionEnum.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Constants.h"
#include "Utils/Random.h"
#include "Utils/Vector.h"

bool Enemy::init(EnemyInfo* enemyDescriptor) {
    float posX = enemyDescriptor->position.x - enemyDescriptor->spriteWidth / 2;
    float posY = enemyDescriptor->position.y - enemyDescriptor->spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));
    _speed = enemyDescriptor->speed;


    _sprite.setScale(enemyDescriptor->scale);
    _sprite.setTexture(*enemyDescriptor->texture);
    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));
    _spriteWidth = enemyDescriptor->spriteWidth;
    _spriteHeight = enemyDescriptor->spriteHeight;
    _spriteColor = _sprite.getColor();

    _maxHealth = enemyDescriptor->maxHealth;
    _currentHealth = _maxHealth;

    _enemyCollider = {
        enemyDescriptor->position.x - TILE_WIDTH / 2,
        enemyDescriptor->position.y - TILE_HEIGHT / 2,
        TILE_WIDTH,
        TILE_HEIGHT
    };

    return true;
}

void Enemy::update(float deltaMilliseconds) {
    if (_isDead) return;

    // Update target position
    calcTargetPosition();

    // Check if vulnerable
    if (_isInvulnerable && _invulnerabilityClock.getElapsedTime().asSeconds() >= INVULNERABILITY_DURATION) {
        _isInvulnerable = false;
    }

    move(deltaMilliseconds);

    checkPlayerCollision();
}

void Enemy::render(sf::RenderWindow& window) {
    // TODO: render dead sprite
    if (_isDead) return;

    sf::Color spriteColor = _sprite.getColor();
    if (_isInvulnerable) {
        spriteColor.a = INVULNERABLE_ALPHA;
        _sprite.setColor(spriteColor);
    }
    else {
        _sprite.setColor(_spriteColor);
    }

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
    sf::RectangleShape collisionRect(sf::Vector2f(_enemyCollider.width, _enemyCollider.height));

    collisionRect.setPosition(_enemyCollider.left, _enemyCollider.top);
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
    _direction = { 0, 0 };

    // Patrol delay control
    if (!_isPatrolling && _patrolDelayClock.getElapsedTime().asSeconds() >= PATROL_DELAY_TIME) {
        _patrolClock.restart();
        _isPatrolling = true;

        // TODO: change the random so direction can be a bitmask
        _faceDirection = static_cast<DirectionEnum>(Random::randomInt(0, 3));
    }

    if (_isPatrolling) {
        // Patrol movement direction
        if (_faceDirection == DirectionEnum::Left) {
            _direction.x = -1.0f;
        }
        if (_faceDirection == DirectionEnum::Right) {
            _direction.x = 1.0f;
        }
        if (_faceDirection == DirectionEnum::Up) {
            _direction.y = -1.0f;
        }
        if (_faceDirection == DirectionEnum::Down) {
            _direction.y = 1.0f;
        }

        // Patrol control
        if (_isPatrolling && _patrolClock.getElapsedTime().asSeconds() >= MAX_PATROL_TIME) {
            _patrolDelayClock.restart();
            _isPatrolling = false;
        }
    }

    _direction = Vector::normalize(_direction);

    float magnitude = Vector::magnitude(_direction);
    if (magnitude > 0.0f) _isMoving = true;
    else _isMoving = false;
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
    _currentTile.x = FRAMES_PER_SECOND * _animationClock.getElapsedTime().asSeconds();

    if (_isMoving) {
        _currentTile.x %= GetMaxMovementTiles();
        _currentTile.y = RUN_ROW;
    }
    else {
        _currentTile.x %= GetMaxIdleTiles();
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
    _enemyCollider.left += velocity.x;
    _enemyCollider.top += velocity.y;

    // Update sprite position
    _sprite.setPosition(_position);
}

void Enemy::attack(float deltaMilliseconds) {
    attackAnimation();
}

void Enemy::attackAnimation() {}

void Enemy::calcTargetPosition() {
    _targetPosition = Player::getInstance()->getPosition();
}

void Enemy::checkPlayerCollision() {
    if (CollisionManager::getInstance()->hasPlayerCollision(_enemyCollider)) {
        DoDamage(Player::getDamageable(), _collisionDamage);
    }
}

void Enemy::getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds) {
    nextBounds = _enemyCollider;
    nextBounds.left += velocity.x;
    nextBounds.top += velocity.y;
}

void Enemy::AddHealth(int amount) {
    if (_isDead) return;

    _currentHealth = std::clamp(_currentHealth + amount, 0, _maxHealth);
}

int Enemy::GetHealth() const {
    return _currentHealth;
}

int Enemy::GetMaxHealth() const {
    return _maxHealth;
}

void Enemy::ReceiveDamage(sf::FloatRect otherCollider, int damageAmount) {
    if (_isDead || _isInvulnerable) return;

    AddHealth(-abs(damageAmount));

    if (_currentHealth == 0) {
        _isDead = true;
        return;
    }

    // TODO: add force depending on 'otherCollider' position
    _sprite.setColor(_damageColor);
    _isInvulnerable = true;
    _invulnerabilityClock.restart();
}

void Enemy::DoDamage(IDamageable* damageable, int damageAmount) {
    damageable->ReceiveDamage(_enemyCollider, damageAmount);
}
