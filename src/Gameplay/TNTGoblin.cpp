#include "Gameplay/TNTGoblin.h"

#include <Gameplay/TorchGoblin.h>

#include "Utils/Vector.h"

bool TNTGoblin::init(EnemyInfo* enemyInfo) {
    auto torchGoblinInfo = static_cast<TNTGoblinInfo*>(enemyInfo);
    _attackDamage = torchGoblinInfo->attackDamage;

    return Enemy::init(enemyInfo);
}

void TNTGoblin::update(float deltaMilliseconds) {
    calcTargetPosition();

    Enemy::update(deltaMilliseconds);
}

void TNTGoblin::render(sf::RenderWindow& window) {
    Enemy::render(window);
}

void TNTGoblin::calcTargetPosition() {
    Enemy::calcTargetPosition();

    _direction = Vector::normalize(_direction);
}
