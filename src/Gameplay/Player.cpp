#include <Gameplay/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

bool Player::init(const PlayerDescriptor& playerDescriptor) {
    _sprite.setTexture(*playerDescriptor.texture);
    _sprite.setPosition(playerDescriptor.position);
    _tileWidth = playerDescriptor.tileWidth;
    _tileHeight = playerDescriptor.tileHeight;

    return true;
}

void Player::update(float deltaMilliseconds) {
    _sprite.setPosition(_position);
}

void Player::render(sf::RenderWindow& window) {
    // Extend this mechanism to be able to support animations
    _sprite.setTextureRect(sf::IntRect(0, 0, _tileWidth, _tileHeight));

    window.draw(_sprite);
}
