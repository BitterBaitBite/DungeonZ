#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>

#include "Enums/DirectionEnum.h"


enum class FaceDirectionX;

class Enemy : public GameObject {
    public:
        struct EnemyDescriptor {
            sf::Vector2f position;
            sf::Texture* texture { nullptr };
            float spriteWidth { .0f };
            float spriteHeight { .0f };

            int maxHealth { 0 };
            sf::Vector2f speed { 0.f, 0.f };
        };

        ~Enemy() override = default;

        bool init(const EnemyDescriptor& enemyDescriptor);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        void debugSprite(sf::RenderWindow& window);

    protected:
        // Movement
        bool _isMoving = false;
        sf::Vector2f _speed { .0f, .0f };
        sf::Vector2f _direction { .0f, .0f };
        sf::FloatRect _collisionBounds { 0.f, 0.f, 0.f, 0.f };

        // Sprite
        sf::Sprite _sprite;
        float _spriteWidth { .0f };
        float _spriteHeight { .0f };

        // Attacks
        const int DEFAULT_TILE_ROW = 2;
        bool _isAttacking { false };


        // Animations
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;
        const int IDLE_ROW = 0;
        const int RUN_ROW = 1;

        float _time { 0.0f };
        sf::Vector2u _currentTile { 0, 0 };
        DirectionEnum _faceDirection { 0 };
        FaceDirectionX _faceDirectionX { 0 };
        FaceDirectionY _faceDirectionY { 0 };

        // Damageable
        bool _isDead { false };
        int _currentHealth { 0 };
        int _maxHealth { 0 };

        // METHODS
        void setPosition(const sf::Vector2f& newPosition) override;
        void getMoveDirection();
        virtual void move(float deltaMilliseconds);
        void setFacingDirection();
        void setMoveAnimation();
        void setMovePosition(float deltaMilliseconds);
        virtual void attack(float deltaMilliseconds);
        virtual void attackAnimation();

        void getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds);

    public:
        void addHealth(int amount);
};
