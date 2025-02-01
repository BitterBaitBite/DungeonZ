#include <Gameplay/TorchGoblin.h>

#include "Utils/Vector.h"

bool TorchGoblin::init(EnemyInfo* enemyInfo) {
    auto torchGoblinInfo = static_cast<TorchGoblinInfo*>(enemyInfo);
    _attackDamage = torchGoblinInfo->attackDamage;

    return Enemy::init(enemyInfo);
}

void TorchGoblin::update(float deltaMilliseconds) {
    calcTargetPosition();

    Enemy::update(deltaMilliseconds);
}

void TorchGoblin::render(sf::RenderWindow& window) {
    Enemy::render(window);
}

void TorchGoblin::calcTargetPosition() {
    Enemy::calcTargetPosition();

    _direction = Vector::normalize(_direction);
}

void TorchGoblin::move(float deltaMilliseconds) {
    getMoveDirection();
    setFacingDirection();
    setMoveAnimation();
    setMovePosition(deltaMilliseconds);
}

void TorchGoblin::setMoveAnimation() {
    Enemy::setMoveAnimation();
}
