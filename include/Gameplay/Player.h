#pragma once

#include <vector>
#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>
#include <tmxlite/Types.hpp>

#include "Enemy.h"
#include "Enums/DirectionEnum.h"
#include "Interfaces/IDamageable.h"
#include "SFML/System/Clock.hpp"
#include "Utils/Constants.h"

class Player : public GameObject, public IDamageable {
    public:
        static Player* getInstance();
        static IDamageable* getDamageable();

        struct PlayerInfo {
            sf::Vector2f scale { 1.f, 1.f };
            sf::Vector2f position { 0.f, 0.f };
            sf::Vector2f speed { .0f, .0f };
            sf::Texture* texture { nullptr };
            float spriteWidth { .0f };
            float spriteHeight { .0f };

            int maxHealth { 0 };
        };

        ~Player() override = default;

        void setPosition(const sf::Vector2f& newPosition) override;

        bool init(const PlayerInfo& playerDescriptor);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

        void debugSprite(sf::RenderWindow& window);
        void debugHealth();

    private:
        static Player* _instance;

        // Movement
        sf::Vector2f _speed { .0f, .0f };
        sf::Vector2f _direction { .0f, .0f };
        sf::FloatRect _playerCollider { 0.f, 0.f, 0.f, 0.f };

        // Sprite
        sf::Sprite _sprite;
        sf::Color _spriteColor { sf::Color::White };
        sf::Color _damageColor { 255, 100, 100 };
        float _spriteWidth { .0f };
        float _spriteHeight { .0f };

        // Damageable
        const float INVULNERABILITY_DURATION = 1.f;
        const uint8_t INVULNERABLE_ALPHA = 120;
        sf::Clock _invulnerabilityClock;
        bool _isInvulnerable { false };
        bool _isDead { false };
        int _currentHealth { 0 };
        int _maxHealth { 0 };

        // Attack
        const int DEFAULT_TILE_ROW = 2;
        const float COMBO_DELAY_TIME = 1.0f;
        const int BASE_ATTACK_DAMAGE = 1;
        const int ATTACK_DAMAGE_MULTIPLIER = 2;

        bool _isAttacking { false };
        int _attackCounter { 0 };
        int _attackDamage[ATTACK_COMBO_COUNT] {};
        sf::Clock _attackAnimationClock;
        sf::Clock _comboDelayClock;
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void attack();
        void resetAttackCounter();
        void resetAttackAnimation();
        void getAttackInput();
        bool attackAnimation();
        void setAttackCollider();
        void checkAttackCollisions();
        void DoDamage(IDamageable* damageable, int damageAmount);

        // Movement
        bool _isMoving { false };
        DirectionEnum _faceDirection { 0 };
        FaceDirectionX _faceDirectionX { 0 };
        FaceDirectionY _faceDirectionY { 0 };

        void move(float deltaMilliseconds);
        void getMoveInput();
        void setFacingDirection();
        void setMoveAnimation();
        void setMovePosition(float deltaMilliseconds);

        // Collisions
        void getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds);
        void BouncePosition(sf::Vector2f velocity);
        void BouncePosition(sf::Vector2f& velocity, std::vector<DirectionEnum> direction);
        void WarpPosition();

        // Animation
        const int IDLE_ROW = 0;
        const int RUN_ROW = 1;
        const int MAX_TILE_FRAMES = 6;
        sf::Clock _animationClock;
        sf::Vector2u _currentTile { 0, 0 };

    protected:
        // Damageable
        void AddHealth(int amount) override;

    public:
        // Damageable
        bool IsDead() const override { return _isDead; }
        int GetHealth() const override;
        int GetMaxHealth() const override;
        void ReceiveDamage(sf::FloatRect otherCollider, int damageAmount) override;

        // Collisions
        sf::FloatRect getCollider() const { return _playerCollider; }
};
