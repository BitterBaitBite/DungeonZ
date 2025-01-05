#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>

class Player : public GameObject {
    public:
        struct PlayerDescriptor {
            sf::Vector2f position;
            sf::Texture* texture { nullptr };
            float tileWidth { .0f };
            float tileHeight { .0f };
        };

        ~Player() override = default;

        bool init(const PlayerDescriptor& playerDescriptor);

        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    private:
        sf::Vector2f _direction { .0f, .0f };
        sf::Vector2f _speed { .0f, .0f };
        sf::Sprite _sprite;
        float _tileWidth { .0f };
        float _tileHeight { .0f };
};
