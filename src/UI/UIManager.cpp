#include "UI/UIManager.h"

#include "Core/AssetManager.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Gameplay/Player.h"


UIManager::~UIManager() {
    delete _mouseCursor;
}

bool UIManager::load() {
    sf::RenderWindow* window = WindowManager::getInstance()->loadWindow();
    _mouseCursor = new MouseCursor(window);
    _playerUI = new PlayerUI();

    // Load font
    _font = AssetManager::getInstance()->loadFont("../Data/Fonts/Jacquard12-Regular.ttf");

    return _mouseCursor != nullptr && _playerUI->load() && _font != nullptr;
}

void UIManager::update(sf::RenderWindow& window, float deltaTime) {
    _playerUI->update(deltaTime);
    _mouseCursor->update(window);

    _enemyCount = DungeonManager::getInstance()->getEnemyCount();
}

void UIManager::render(sf::RenderWindow& window, bool hasGameStarted) {
    if (hasGameStarted) {
        _playerUI->render(window);

        sf::Text currentEnemyCount;
        currentEnemyCount.setFont(*_font);
        currentEnemyCount.setString(std::to_string(_enemyCount.x));
        currentEnemyCount.setCharacterSize(40);
        currentEnemyCount.setFillColor(sf::Color::White);
        currentEnemyCount.setOutlineColor(sf::Color::Black);
        currentEnemyCount.setOutlineThickness(2.f);
        currentEnemyCount.setPosition(
            window.getSize().x / 2 - currentEnemyCount.getLocalBounds().width / 2 - 40,
            50
        );
        window.draw(currentEnemyCount);

        sf::Text barSlice;
        barSlice.setFont(*_font);
        barSlice.setString("/");
        barSlice.setCharacterSize(40);
        barSlice.setFillColor(sf::Color::White);
        barSlice.setOutlineColor(sf::Color::Black);
        barSlice.setOutlineThickness(2.f);
        barSlice.setPosition(
            window.getSize().x / 2 - barSlice.getLocalBounds().width / 2,
            50
        );
        window.draw(barSlice);

        sf::Text totalEnemyCount;
        totalEnemyCount.setFont(*_font);
        totalEnemyCount.setString(std::to_string(_enemyCount.y));
        totalEnemyCount.setCharacterSize(40);
        totalEnemyCount.setFillColor(sf::Color::White);
        totalEnemyCount.setOutlineColor(sf::Color::Black);
        totalEnemyCount.setOutlineThickness(2.f);
        totalEnemyCount.setPosition(
            window.getSize().x / 2 - totalEnemyCount.getLocalBounds().width / 2 + 40,
            50
        );
        window.draw(totalEnemyCount);
    }

    _mouseCursor->render(window);
}

void UIManager::reset() {
    _playerUI->reset();
}

bool UIManager::HasPressedRestart() const {
    if (_playerUI == nullptr) return true;

    if (_playerUI->HasPressedRestart()) {
        return true;
    }

    return false;
}
