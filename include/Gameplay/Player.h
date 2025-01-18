#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>

#include "Enums/FaceDirection.h"

class Player : public GameObject {
    public:
        struct PlayerDescriptor {
            sf::Vector2f scale;
            sf::Vector2f position;
            sf::Vector2f speed { .0f, .0f };
            sf::Texture* texture { nullptr };
            float tileWidth { .0f };
            float tileHeight { .0f };
        };

        ~Player() override = default;

        void setPosition(const sf::Vector2f& newPosition) override {
            _position = newPosition;
            _sprite.setPosition(newPosition);
        }

        bool init(const PlayerDescriptor& playerDescriptor);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

        void calcMoveInput();
        void calcFacingDirection();

    private:
        sf::Vector2f _direction { .0f, .0f };
        sf::Vector2f _speed { .0f, .0f };
        sf::Sprite _sprite;
        float _tileWidth { .0f };
        float _tileHeight { .0f };

        bool _isMoving = false;
        FaceDirection _faceDirection { 0 };

        void move(float deltaMilliseconds);
};
