#pragma once

#include <vector>
#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>
#include <tmxlite/Types.hpp>

#include "Enums/DirectionEnum.h"

class Player : public GameObject {
    public:
        struct PlayerDescriptor {
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

        bool init(const PlayerDescriptor& playerDescriptor);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

        void debugSprite(sf::RenderWindow& window);

    private:
        // Movement
        sf::Vector2f _speed { .0f, .0f };
        sf::Vector2f _direction { .0f, .0f };
        sf::FloatRect _collisionBounds { 0.f, 0.f, 0.f, 0.f };

        // Sprite
        sf::Sprite _sprite;
        float _spriteWidth { .0f };
        float _spriteHeight { .0f };

        // Damageable
        bool _isDead { false };
        int _currentHealth { 0 };
        int _maxHealth { 0 };
        void addHealth(int amount);

        // ATTACK
        const int DEFAULT_TILE_ROW = 2;
        const int MAX_ATTACK_COUNT = 2;
        const float MAX_COMBO_DELAY = 1.0f;

        bool _isAttacking { false };
        int _attackCounter { 0 };
        float _attackTime { 0.f };
        float _comboDelayTime { 0.0f };
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void attack(float deltaMilliseconds);
        void resetAttackCounter();
        void resetAttackAnimation();
        void getAttackInput();
        void attackAnimation();
        void setAttackCollider();

        // MOVEMENT
        bool _isMoving { false };
        DirectionEnum _faceDirection { 0 };
        FaceDirectionX _faceDirectionX { 0 };
        FaceDirectionY _faceDirectionY { 0 };

        void move(float deltaMilliseconds);
        void getMoveInput();
        void setFacingDirection();
        void setMoveAnimation();
        void getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds);
        void BouncePosition(sf::Vector2f velocity);
        void BouncePosition(sf::Vector2f& velocity, std::vector<DirectionEnum> direction);
        void WarpPosition();
        void setMovePosition(float deltaMilliseconds);

        // ANIMATION
        const int MAX_TILE_FRAMES = 6;
        float _time { 0.f };
        sf::Vector2u _currentTile { 0, 0 };
};
