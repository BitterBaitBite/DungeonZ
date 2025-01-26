#pragma once

#include <Gameplay/Enemy.h>

class Villager : public Enemy {
    public:
        struct VillagerDescriptor : EnemyDescriptor {
            sf::Vector2f speed { .0f, .0f };
        };

        bool init(const VillagerDescriptor& enemyDescriptor);

        void update(float deltaMilliseconds) override;

        void calcTargetPosition();

    private:
        sf::Vector2f _direction { .0f, .0f };
        sf::Vector2f _speed { .0f, .0f };
};
