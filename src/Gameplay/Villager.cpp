#include <queue>
#include <Gameplay/Villager.h>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Pathfinding.h"
#include "Utils/Random.h"
#include "Utils/Vector.h"

bool Villager::init(EnemyInfo* enemyInfo) {
    auto villagerInfo = static_cast<VillagerInfo*>(enemyInfo);
    _attackDamage = villagerInfo->attackDamage;

    return Enemy::init(enemyInfo);
}


void Villager::update(float deltaMilliseconds) {
    if (_isDead) return;

    // Check if vulnerable
    if (_isInvulnerable && _invulnerabilityClock.getElapsedTime().asSeconds() >= INVULNERABILITY_DURATION) {
        _isInvulnerable = false;
    }

    checkPlayerCollision();
    if (_isAttacking) {
        attack();
    }

    if (!_isAttacking) {
        move(deltaMilliseconds);
    }
}

void Villager::attack() {
    attackAnimation();
    checkAttackCollisions();
}

bool Villager::attackAnimation() {
    bool animationFinished = false;
    _currentTile.x = FRAMES_PER_SECOND * _attackAnimationClock.getElapsedTime().asSeconds();

    if (_currentTile.x == 3) {
        setAttackCollider();
    }

    if (_currentTile.x >= MAX_ATTACK_TILES) {
        _currentTile.x %= MAX_ATTACK_TILES;
        _isAttacking = false;
        animationFinished = true;
    }

    int tileColumn = D_SPRITE_ATTACK_ROW + ATTACK_TYPE;

    _currentTile.y = tileColumn;

    return animationFinished;
}

void Villager::resetAttackAnimation() {
    _currentTile.x = 0;
    _attackAnimationClock.restart();
}

void Villager::setAttackCollider() {
    sf::FloatRect attackColliderRect = {
        _enemyCollider.left - _enemyCollider.width / 2,
        _enemyCollider.top - _enemyCollider.height / 2,
        TILE_WIDTH,
        TILE_HEIGHT * 2,
    };

    switch (_faceDirectionX) {
        case DirectionX::Right:
            attackColliderRect.left = _enemyCollider.left + _enemyCollider.width - _enemyCollider.width / 2;
            break;

        case DirectionX::Left:
            attackColliderRect.left = _enemyCollider.left - _enemyCollider.width / 2;
            break;
    }

    _attackCollider = attackColliderRect;
}

void Villager::checkAttackCollisions() {
    if (CollisionManager::getInstance()->hasPlayerCollision(_attackCollider)) {
        DoDamage(Player::getInstance(), _attackDamage + ATTACK_TYPE);
    }
}

void Villager::calcTargetPosition() {
    Enemy::calcTargetPosition();

    if (_targetPosition.x < 0 || _targetPosition.y < 0) {
        _hasCloseTarget = false;
        return;
    }

    float targetDistance = Vector::distance(_position, _targetPosition);
    _hasCloseTarget = targetDistance <= TARGET_RANGE;
}

void Villager::checkPlayerCollision() {
    if (_isAttacking) return;

    if (CollisionManager::getInstance()->hasPlayerCollision(_enemyCollider)) {
        _isAttacking = true;
        resetAttackAnimation();
    }

    _attackCollider = { 0.f, 0.f, 0.f, 0.f };
}

void Villager::debugSprite(sf::RenderWindow& window) {
    Enemy::debugSprite(window);

    sf::CircleShape closeTargetCircle(TARGET_RANGE);

    closeTargetCircle.setPosition({ _position.x - TARGET_RANGE / 2, _position.y - TARGET_RANGE / 2 });
    closeTargetCircle.setOutlineColor(sf::Color::Red);
    closeTargetCircle.setOutlineThickness(2.f);
    closeTargetCircle.setFillColor(sf::Color::Transparent);

    // Attack collider
    sf::RectangleShape attackRect(sf::Vector2f(_attackCollider.width, _attackCollider.height));

    attackRect.setPosition(_attackCollider.left, _attackCollider.top);
    attackRect.setOutlineColor(sf::Color::Magenta);
    attackRect.setOutlineThickness(2.f);
    attackRect.setFillColor(sf::Color::Transparent);

    window.draw(closeTargetCircle);
    window.draw(attackRect);
}

void Villager::move(float deltaMilliseconds) {
    calcTargetPosition();

    if (_hasCloseTarget) {
        movePathToTarget(deltaMilliseconds);
    }
    else {
        getMoveDirection();
        setMovePosition(deltaMilliseconds);
    }

    setFacingDirection();
    setMoveAnimation();
}

void Villager::resetPathToTarget() {
    sf::Vector2i targetTile = {
        static_cast<int>((_targetPosition.x + _spriteWidth / 2) / TILE_WIDTH),
        static_cast<int>((_targetPosition.y + _spriteHeight / 2) / TILE_HEIGHT)
    };

    sf::Vector2i currentTile = {
        static_cast<int>((_position.x + _spriteWidth / 2) / TILE_WIDTH),
        static_cast<int>((_position.y + _spriteHeight / 2) / TILE_HEIGHT)
    };

    _pathToTarget = Pathfinding::getPath(
        currentTile,
        targetTile,
        DungeonManager::getInstance()->getRoomCostGrid()
    );

    _currentTarget = 0;
    _repathClock.restart();
}

void Villager::movePathToTarget(float deltaMilliseconds) {
    // Added clock to get a new path every *REPATH_TIME* time (.5f default)
    if (_pathToTarget.empty() || _repathClock.getElapsedTime().asSeconds() >= REPATH_TIME) {
        resetPathToTarget();
    }

    if (_currentTarget >= _pathToTarget.size()) {
        _pathToTarget.clear();
        _isMoving = false;
        return;
    }

    sf::Vector2f targetPos(_pathToTarget[_currentTarget].x * TILE_WIDTH - TILE_WIDTH,
                           _pathToTarget[_currentTarget].y * TILE_HEIGHT - TILE_HEIGHT);
    sf::Vector2f currentPos = _sprite.getPosition();
    _direction = targetPos - currentPos;

    float distance = Vector::magnitude(_direction);
    if (distance < _speed.x * deltaMilliseconds) {
        _sprite.setPosition(targetPos);
        _currentTarget++;
        _isMoving = false;
    }
    else {
        _direction /= distance; // Normalize

        auto velocity = sf::Vector2f(_direction.x * _speed.x * deltaMilliseconds,
                                     _direction.y * _speed.y * deltaMilliseconds);

        _position.x += velocity.x;
        _position.y += velocity.y;

        _enemyCollider.left += velocity.x;
        _enemyCollider.top += velocity.y;

        _sprite.move(velocity);

        float magnitude = Vector::magnitude(_direction);
        if (magnitude > 0.0f) _isMoving = true;
        else _isMoving = false;
    }
}

void Villager::getMoveDirection() {
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
