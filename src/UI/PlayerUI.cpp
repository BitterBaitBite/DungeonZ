#include "UI/PlayerUI.h"

#include <valarray>

#include "Core/AssetManager.h"
#include "Core/DungeonManager.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Keyboard.hpp"


PlayerUI::PlayerUI() {
    auto size = sf::Vector2f(BACKGROUND_ROW_SIZE * TILE_WIDTH / 4, TILE_HEIGHT / 3);
    auto position = sf::Vector2f(BACKGROUND_ROW_SIZE * TILE_WIDTH / 2 - size.x / 2,
                                 BACKGROUND_COL_SIZE * TILE_HEIGHT - 2 * size.y);

    _currentHealthUI.setFillColor(_healthColor);
    _currentHealthUI.setOutlineColor(sf::Color::Transparent);
    _currentHealthUI.setSize(size);
    _currentHealthUI.setPosition(position);

    _maxHealthUI.setFillColor(sf::Color::Transparent);
    _maxHealthUI.setOutlineColor(sf::Color::Black);
    _maxHealthUI.setOutlineThickness(2.f);
    _maxHealthUI.setSize(size);
    _maxHealthUI.setPosition(position);
}

void PlayerUI::setDeathText(float& textPosX, float& textPosY) {
    _deathText.setFont(*_font);
    _deathText.setString("You're dead!");
    _deathText.setCharacterSize(80);
    _deathText.setFillColor(sf::Color::Red);
    _deathText.setOutlineColor(sf::Color::Black);
    _deathText.setOutlineThickness(1.f);
    textPosX = _maxHealthUI.getPosition().x + _maxHealthUI.getSize().x / 2 - _deathText.getGlobalBounds().width / 2;
    textPosY = BACKGROUND_COL_SIZE * TILE_HEIGHT / 2 - _deathText.getGlobalBounds().height;
    _deathText.setPosition({ textPosX, textPosY });
}

void PlayerUI::setRestartText(float& textPosX, float textPosY) {
    _restartText.setFont(*_font);
    _restartText.setString("Press SPACE to continue...");
    _restartText.setCharacterSize(50);
    _restartText.setFillColor(sf::Color(255, 0, 0, 0));
    _restartText.setOutlineColor(sf::Color(0, 0, 0, 0));
    _restartText.setOutlineThickness(1.f);
    textPosX =
        _maxHealthUI.getPosition().x + _maxHealthUI.getSize().x / 2 - _restartText.getGlobalBounds().width / 2;
    _restartText.setPosition({ textPosX, textPosY + _deathText.getGlobalBounds().height * 2 });
}

bool PlayerUI::load() {
    // Load font
    _font = AssetManager::getInstance()->loadFont("../Data/Fonts/Jacquard12-Regular.ttf");
    if (_font == nullptr) return false;

    float textPosX;
    float textPosY;
    setDeathText(textPosX, textPosY);
    setRestartText(textPosX, textPosY);

    return true;
}

void PlayerUI::update(float deltaMilliseconds) {
    Player* player = Player::getInstance();

    if (!_isDead && player->IsDead()) {
        _restartTextClock.restart();
        _isDead = true;
        return;
    }

    sf::Color color = _restartText.getFillColor();
    sf::Color outlineColor = _restartText.getOutlineColor();
    if (_restartTextAlpha) {
        color.a = std::clamp<uint8_t>(color.a - 1, 0, 255);
        outlineColor.a = color.a;
        if (color.a <= 0) _restartTextAlpha = false;
    }
    else {
        color.a = std::clamp<uint8_t>(color.a + 1, 0, 255);
        outlineColor.a = color.a;
        if (color.a >= 255) _restartTextAlpha = true;
    }
    _restartText.setFillColor(color);
    _restartText.setOutlineColor(outlineColor);

    if (!_hasPressedRestart && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        _hasPressedRestart = true;
    }

    float maxWidth = _maxHealthUI.getSize().x;
    int maxHealth = player->GetMaxHealth();
    int currentHealth = player->GetHealth();

    if (player->getIsBurning()) {
        _currentHealthUI.setFillColor(_burningColor);
    }
    else {
        _currentHealthUI.setFillColor(_healthColor);
    }

    _currentHealthUI.setSize(sf::Vector2f(currentHealth * maxWidth / maxHealth, _maxHealthUI.getSize().y));
}

void PlayerUI::render(sf::RenderWindow& window) {
    if (_isDead) {
        window.draw(_deathText);

        if (_restartTextClock.getElapsedTime().asSeconds() > RESTART_TEXT_DELAY) {
            window.draw(_restartText);
        }

        return;
    }

    window.draw(_currentHealthUI);
    window.draw(_maxHealthUI);
}

void PlayerUI::reset() {
    _isDead = false;
    _hasDelayStarted = false;
    _restartTextAlpha = true;
    _hasPressedRestart = false;
}
