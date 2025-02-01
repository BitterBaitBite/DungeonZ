#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>

#include "Enums/DirectionEnum.h"
#include "Interfaces/IDamageable.h"
#include "SFML/System/Clock.hpp"

namespace sf {
    class Clock;
}

enum class FaceDirectionX;

class Enemy : public GameObject, public IDamageable {
    public:
        struct EnemyInfo {
            sf::Vector2f scale { 1.f, 1.f };
            sf::Vector2f position;
            sf::Vector2f speed { 0.f, 0.f };
            sf::Texture* texture { nullptr };
            float spriteWidth { .0f };
            float spriteHeight { .0f };

            int maxHealth { 0 };

            int collisionDamage { 1 };
        };

        ~Enemy() override = default;

        virtual bool init(EnemyInfo* enemyDescriptor);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        virtual void debugSprite(sf::RenderWindow& window);

    protected:
        // Movement
        bool _isMoving = false;
        sf::Vector2f _speed { .0f, .0f };
        sf::Vector2f _direction { .0f, .0f };
        sf::FloatRect _enemyCollider { 0.f, 0.f, 0.f, 0.f };

        // Sprite
        sf::Sprite _sprite;
        sf::Color _spriteColor { sf::Color::White };
        sf::Color _damageColor { 255, 100, 100 };
        float _spriteWidth { .0f };
        float _spriteHeight { .0f };

        // Attacks
        const int DEFAULT_TILE_ROW = 2;
        bool _isAttacking { false };
        int _collisionDamage = 1;
        sf::Vector2f _targetPosition { 0, 0 };

        // Animations
        const int MAX_IDLE_TILES = 6;
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;
        const int IDLE_ROW = 0;
        const int RUN_ROW = 1;
        sf::Clock _animationClock;
        sf::Vector2u _currentTile { 0, 0 };
        DirectionEnum _faceDirection { 0 };
        FaceDirectionX _faceDirectionX { 0 };
        FaceDirectionY _faceDirectionY { 0 };

        // Patrol
        const float MAX_PATROL_TIME { 1.5f };
        const float PATROL_DELAY_TIME { 2.5f };
        bool _hasTarget { false };
        bool _isPatrolling { false };
        sf::Clock _patrolClock;
        sf::Clock _patrolDelayClock;

        // Damageable
        const float INVULNERABILITY_DURATION = .5f;
        const uint8_t INVULNERABLE_ALPHA = 120;
        sf::Clock _invulnerabilityClock;
        bool _isInvulnerable { false };
        bool _isDead { false };
        int _currentHealth { 0 };
        int _maxHealth { 0 };

        /* METHODS */
        // Movement
        virtual void move(float deltaMilliseconds);
        virtual void getMoveDirection();
        void setFacingDirection();
        virtual void setMoveAnimation();
        void setMovePosition(float deltaMilliseconds);
        void setPosition(const sf::Vector2f& newPosition) override;

        // Attack
        virtual void attack(float deltaMilliseconds);
        virtual void attackAnimation();
        virtual void calcTargetPosition();

        // Collisions
        void checkPlayerCollision();
        void getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds);

        // Damageable
        void AddHealth(int amount) override;

        // Animations
        virtual int GetMaxIdleTiles() const { return MAX_IDLE_TILES; }
        virtual int GetMaxMovementTiles() const { return MAX_MOVEMENT_TILES; }

    public:
        // Damageable
        bool IsDead() const override { return _isDead; }
        int GetHealth() const override;
        int GetMaxHealth() const override;
        void ReceiveDamage(sf::FloatRect otherCollider, int damageAmount) override;
        void DoDamage(IDamageable* damageable, int damageAmount);

        // Collisions
        sf::FloatRect getCollider() const { return _enemyCollider; }
};
