#include <Gameplay/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor) {
    _sprite.setTexture(*enemyDescriptor.texture);
    _sprite.setPosition(enemyDescriptor.position);
    _tileWidth = enemyDescriptor.tileWidth;
    _tileHeight = enemyDescriptor.tileHeight;

    return true;
}

void Enemy::update(float deltaMilliseconds) {
    _sprite.setPosition(_position);
}

void Enemy::render(sf::RenderWindow& window) {
    // Extend this mechanism to be able to support animations
    _sprite.setTextureRect(sf::IntRect(0, 0, _tileWidth, _tileHeight));

    window.draw(_sprite);
}
