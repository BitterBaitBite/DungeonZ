#include "Core/WindowManager.h"

#include "Utils/Constants.h"
WindowManager* WindowManager::_instance { nullptr };

WindowManager* WindowManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new WindowManager();
    }

    return _instance;
}

WindowManager::~WindowManager() {
    delete _window;

    delete _instance;
}

sf::RenderWindow* WindowManager::loadWindow() {
    if (_window == nullptr) {
        _window = new sf::RenderWindow(
            {
                BACKGROUND_ROW_SIZE * TILE_WIDTH,
                BACKGROUND_COL_SIZE * TILE_HEIGHT
            },
            GAME_TITLE
        );

        _window->setFramerateLimit(FRAME_RATE_LIMIT);
    }

    return _window;
}

sf::RenderWindow* WindowManager::loadWindow(sf::VideoMode videoMode, std::string title, uint32_t frameRateLimit) {
    if (_window == nullptr) {
        _window = new sf::RenderWindow(videoMode, title);
        _window->setFramerateLimit(frameRateLimit);
    }

    return _window;
}

sf::Vector2u WindowManager::getWindowSize() const {
    if (_window == nullptr) {
        return sf::Vector2u(0, 0);
    }

    return _window->getSize();
}
