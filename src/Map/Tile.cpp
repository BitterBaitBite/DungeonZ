#include "SFML/Graphics/RenderWindow.hpp"

#include "Map/Tile.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Constants.h"

void Tile::init(const TileDescriptor& tileDescriptor) {
    _sprite.setPosition(tileDescriptor.position);
    _sprite.setScale(tileDescriptor.scale);
    _sprite.setTexture(*tileDescriptor.texture);
    _spriteWidth = tileDescriptor.spriteWidth;
    _spriteHeight = tileDescriptor.spriteHeight;
}

void Tile::debugRender(sf::RenderWindow& window) {
    const sf::FloatRect spriteBounds = _sprite.getGlobalBounds();
    sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
    boundsRect.setOutlineColor(sf::Color::Cyan);
    boundsRect.setOutlineThickness(2.f);
    boundsRect.setFillColor(sf::Color::Transparent);

    window.draw(boundsRect);
}

void Tile::render(sf::RenderWindow& window) {
    sf::Vector2f tilePosition { _currentTile.x * _spriteWidth, _currentTile.y * _spriteHeight };
    _sprite.setTextureRect(sf::IntRect(tilePosition.x, tilePosition.y, _spriteWidth, _spriteHeight));

    window.draw(_sprite);

    // debugRender(window);
}

void Tile::update(float deltaMilliseconds) {
    _time += deltaMilliseconds;

    animate();
}

void Tile::animate() {
    _currentTile.x = FRAMES_PER_SECOND * _time / 1000.0f;
    _currentTile.x %= MAX_TILE_FRAMES;

    _currentTile.y = 0;
}
