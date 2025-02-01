#pragma once

#include <Gameplay/Enemy.h>

class TorchGoblin : public Enemy {
    public:
        struct TorchGoblinInfo : EnemyInfo {
            int attackDamage { 1 };
        };

        bool init(EnemyInfo* enemyInfo) override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    private:
        // Animations
        const int MAX_IDLE_TILES = 7;
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;

        int GetMaxIdleTiles() const override { return MAX_IDLE_TILES; }
        int GetMaxMovementTiles() const override { return MAX_MOVEMENT_TILES; }

        // Attack
        int _attackDamage { 1 };
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void calcTargetPosition() override;

        // Movement
        void move(float deltaMilliseconds) override;
        void setMoveAnimation() override;
};
