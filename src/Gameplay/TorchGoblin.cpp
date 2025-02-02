#include <Gameplay/TorchGoblin.h>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Constants.h"
#include "Utils/Pathfinding.h"
#include "Utils/Random.h"
#include "Utils/Vector.h"

bool TorchGoblin::init(EnemyInfo* enemyInfo) {
    auto torchGoblinInfo = static_cast<TorchGoblinInfo*>(enemyInfo);
    _attackDamage = torchGoblinInfo->attackDamage;
    _fireDamage = torchGoblinInfo->fireDamage;
    _fireDuration = torchGoblinInfo->fireDuration;

    return Enemy::init(enemyInfo);
}

void TorchGoblin::update(float deltaMilliseconds) {
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

void TorchGoblin::attack() {
    attackAnimation();
    checkAttackCollisions();
}

bool TorchGoblin::attackAnimation() {
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

    int tileColumn = D_SPRITE_ATTACK_ROW + static_cast<int>(_faceDirection);
    if (_faceDirection == DirectionEnum::Left) tileColumn = D_SPRITE_ATTACK_ROW;

    _currentTile.y = tileColumn;

    return animationFinished;
}

void TorchGoblin::resetAttackAnimation() {
    _currentTile.x = 0;
    _attackAnimationClock.restart();
}

void TorchGoblin::setAttackCollider() {
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

void TorchGoblin::checkAttackCollisions() {
    if (CollisionManager::getInstance()->hasPlayerCollision(_attackCollider)) {
        Player* player = Player::getInstance();
        DoDamage(player, _attackDamage);
        player->setInFire(_fireDuration, _fireDamage);
    }
}

void TorchGoblin::checkPlayerCollision() {
    if (_isAttacking) return;

    if (CollisionManager::getInstance()->hasPlayerCollision(_enemyCollider)) {
        _isAttacking = true;
        resetAttackAnimation();
    }

    _attackCollider = { 0.f, 0.f, 0.f, 0.f };
}

void TorchGoblin::move(float deltaMilliseconds) {
    // calcTargetPosition();
    //
    // movePathToTarget(deltaMilliseconds);
    // setFacingDirection();
    // setMoveAnimation();

    calcTargetPosition();

    if (_targetPosition.x >= 0 && _targetPosition.y >= 0) {
        movePathToTarget(deltaMilliseconds);
    }
    else {
        getMoveDirection();
        setMovePosition(deltaMilliseconds);
    }

    setFacingDirection();
    setMoveAnimation();
}

void TorchGoblin::resetPathToTarget() {
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

void TorchGoblin::movePathToTarget(float deltaMilliseconds) {
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
