#pragma once
#include "Enemy.h"

// TODO
class TNTGoblin : Enemy {
    public:
        struct TNTGoblinInfo : EnemyInfo {
            int attackDamage { 1 };
        };

        bool init(EnemyInfo* enemyInfo) override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    private:
        int _attackDamage { 1 };

        void calcTargetPosition() override;
};
