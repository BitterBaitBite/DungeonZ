#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>

#include "Interfaces/IDamageable.h"

class Enemy : public GameObject {
    public:
        struct EnemyDescriptor {
            sf::Vector2f position;
            sf::Texture* texture { nullptr };
            float tileWidth { .0f };
            float tileHeight { .0f };
        };

        ~Enemy() override = default;

        bool init(const EnemyDescriptor& enemyDescriptor);

        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    protected:
        sf::Sprite _sprite;
        float _tileWidth { .0f };
        float _tileHeight { .0f };
};
