#pragma once

#include <Gameplay/Enemy.h>

#include "SFML/Graphics/VertexArray.hpp"
#include "Utils/Constants.h"
#include "Utils/Random.h"

class Villager : public Enemy {
    public:
        struct VillagerInfo : EnemyInfo {
            int attackDamage { 1 };
        };

        bool init(EnemyInfo* enemyInfo) override;
        void update(float deltaMilliseconds) override;

    private:
        // Animations
        const int MAX_IDLE_TILES = 6;
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;
        const int ATTACK_TYPE = Random::randomInt(0, 1);

        int GetMaxIdleTiles() const override { return MAX_IDLE_TILES; }
        int GetMaxMovementTiles() const override { return MAX_MOVEMENT_TILES; }

        // Attack
        const float TARGET_RANGE = 200.0f;
        int _attackDamage { 1 };
        sf::Clock _attackAnimationClock;
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void attack() override;
        bool attackAnimation() override;
        void resetAttackAnimation();
        void setAttackCollider() override;
        void checkAttackCollisions() override;
        void calcTargetPosition() override;
        void checkPlayerCollision() override;

        // Render
        void debugSprite(sf::RenderWindow& window) override;

        // Movement
        const float REPATH_TIME = 0.5f;
        sf::Clock _repathClock;
        size_t _currentTarget = 0;
        bool _hasCloseTarget { false };
        std::vector<sf::Vector2i> _pathToTarget;

        void move(float deltaMilliseconds) override;
        void getMoveDirection() override;
        void resetPathToTarget();
        void movePathToTarget(float deltaMilliseconds);
};
