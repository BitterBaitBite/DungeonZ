#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

namespace sf {
    class RenderWindow;
}

class Tile {
    public:
        struct TileDescriptor {
            // The position represents the top-left corner relative position of a 64x64 tile.
            // The actual sprite render can be offset and bigger for design/animation purposes
            sf::Vector2f position;
            sf::Vector2f scale { 1.f, 1.f };
            sf::Texture* texture { nullptr };
            float spriteWidth { .0f };
            float spriteHeight { .0f };
        };

        ~Tile() = default;

        void init(const TileDescriptor& tileDescriptor);
        void debugRender(sf::RenderWindow& window);
        void render(sf::RenderWindow& window);
        void update(float deltaMilliseconds);
        void animate();

    protected:
        sf::Sprite _sprite;
        float _spriteWidth { .0f };
        float _spriteHeight { .0f };

        // ANIMATION
        const int MAX_TILE_FRAMES = 8;
        float _time { 0.f };
        sf::Vector2u _currentTile { 0, 0 };
};
