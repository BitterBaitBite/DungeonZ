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
            sf::Vector2f position;
            sf::Vector2f scale;
            sf::Texture* texture { nullptr };
            float tileWidth { .0f };
            float tileHeight { .0f };
        };

        void init(const TileDescriptor& tileDescriptor);
        void render(sf::RenderWindow& window);

    private:
        sf::Sprite _sprite;
        float _tileWidth { .0f };
        float _tileHeight { .0f };
};
