#include "UI/MouseCursor.h"

#include "Core/AssetManager.h"
#include "SFML/Window/Mouse.hpp"


MouseCursor::MouseCursor(sf::RenderWindow* window) {
    sf::Texture* defaultCursorTexture = AssetManager::getInstance()->loadTexture(
        "../Data/Images/UI/Pointers/01.png");
    sf::Texture* selectCursorTexture = AssetManager::getInstance()->loadTexture(
        "../Data/Images/UI/Pointers/02.png");

    if (defaultCursorTexture != nullptr && selectCursorTexture != nullptr) {
        window->setMouseCursorVisible(false);

        _defaultCursor = new sf::Sprite(*defaultCursorTexture);
        _selectCursor = new sf::Sprite(*selectCursorTexture);
    }
}

MouseCursor::~MouseCursor() {
    delete _defaultCursor;
    delete _selectCursor;
}

void MouseCursor::update(sf::RenderWindow& window) {
    _pos = sf::Vector2i(sf::Mouse::getPosition(window));

    if (_isSelect) {
        _activeSprite = *_selectCursor;
    }
    else {
        _activeSprite = *_defaultCursor;
    }
}

void MouseCursor::render(sf::RenderWindow& window) {
    auto spritePos = sf::Vector2f(_pos.x - _activeSprite.getGlobalBounds().width / 3,
                                  _pos.y - _activeSprite.getGlobalBounds().height / 3);
    _activeSprite.setPosition(spritePos);

    window.draw(_activeSprite);
}
