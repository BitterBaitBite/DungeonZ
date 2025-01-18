#include "SFML/Graphics/RenderWindow.hpp"

#include "Core/Tile.h"

void Tile::init(const TileDescriptor& tileDescriptor) {
    _sprite.setPosition(tileDescriptor.position);
    _sprite.setScale(tileDescriptor.scale);
    _sprite.setTexture(*tileDescriptor.texture);
    _tileWidth = tileDescriptor.tileWidth;
    _tileHeight = tileDescriptor.tileHeight;
}

void Tile::render(sf::RenderWindow& window) {
    // Extend this mechanism to be able to support animations
    _sprite.setTextureRect(sf::IntRect(0, 0, _tileWidth, _tileHeight));

    window.draw(_sprite);
}
