#include "UI/UIManager.h"

#include "Core/WindowManager.h"
#include "Gameplay/Player.h"


UIManager::~UIManager() {
    delete _mouseCursor;
}

bool UIManager::load() {
    sf::RenderWindow* window = WindowManager::getInstance()->loadWindow();
    _mouseCursor = new MouseCursor(window);
    _playerUI = new PlayerUI();


    return _mouseCursor != nullptr && _playerUI->load();
}

void UIManager::update(sf::RenderWindow& window, float deltaTime) {
    _playerUI->update(deltaTime);
    _mouseCursor->update(window);
}

void UIManager::render(sf::RenderWindow& window, bool hasGameStarted) {
    if (hasGameStarted) {
        _playerUI->render(window);
    }

    _mouseCursor->render(window);
}
