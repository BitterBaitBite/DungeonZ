#pragma once

#include <Gameplay/Enemy.h>

class Zombie : public Enemy {
    public:
        struct ZombieDescriptor : EnemyDescriptor {
            sf::Vector2f speed { .0f, .0f };
        };

        bool init(const ZombieDescriptor& enemyDescriptor);

        void update(float deltaMilliseconds) override;

        void calcMoveInput();

    private:
        sf::Vector2f _direction { .0f, .0f };
        sf::Vector2f _speed { .0f, .0f };
};
