#include "Gameplay/DynamiteGoblin.h"

#include "Utils/Vector.h"

bool DynamiteGoblin::init(EnemyInfo* enemyInfo) {
    auto dynamiteGoblinInfo = static_cast<DynamiteGoblinInfo*>(enemyInfo);
    _attackDamage = dynamiteGoblinInfo->attackDamage;

    return Enemy::init(enemyInfo);
}

void DynamiteGoblin::update(float deltaMilliseconds) {
    calcTargetPosition();

    Enemy::update(deltaMilliseconds);
}

void DynamiteGoblin::render(sf::RenderWindow& window) {
    Enemy::render(window);
}

void DynamiteGoblin::calcTargetPosition() {
    Enemy::calcTargetPosition();

    _direction = Vector::normalize(_direction);
}
