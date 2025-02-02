#pragma once

#include <Gameplay/Enemy.h>

class TorchGoblin : public Enemy {
    public:
        struct TorchGoblinInfo : EnemyInfo {
            int attackDamage { 1 };
            int fireDamage { 1 };
            float fireDuration { 1.5f };
        };

        bool init(EnemyInfo* enemyInfo) override;
        void update(float deltaMilliseconds) override;

    private:
        // Animations
        const int MAX_IDLE_TILES = 7;
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;

        int GetMaxIdleTiles() const override { return MAX_IDLE_TILES; }
        int GetMaxMovementTiles() const override { return MAX_MOVEMENT_TILES; }

        // Attack
        int _attackDamage { 1 };
        int _fireDamage { 1 };
        float _fireDuration { 1.5f };
        sf::Clock _attackAnimationClock;
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void attack() override;
        bool attackAnimation() override;
        void resetAttackAnimation();
        void setAttackCollider() override;
        void checkAttackCollisions() override;
        void checkPlayerCollision() override;

        // Movement
        const float REPATH_TIME = 0.5f;
        sf::Clock _repathClock;
        size_t _currentTarget = 0;
        std::vector<sf::Vector2i> _pathToTarget;

        void move(float deltaMilliseconds) override;
        void resetPathToTarget();
        void movePathToTarget(float deltaMilliseconds);
};
