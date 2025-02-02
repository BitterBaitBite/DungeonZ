#include <cassert>
#include <Core/Game.h>
#include <Core/WindowManager.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <UI/MainMenu.h>


bool Game::init() {
    assert(_window == nullptr && _world == nullptr, "Game is already initialized, we are about to leak memory");

    _window = WindowManager::getInstance()->loadWindow();
    bool loadOk = _window != nullptr;

    _mainMenu = new MainMenu();
    loadOk &= _mainMenu->load();

    _world = new World();
    loadOk &= _world->load();

    return loadOk;
}

Game::~Game() {
    delete _world;
    delete _mainMenu;
    delete _window;
}

bool Game::isRunning() const {
    return _window->isOpen();
}

void Game::update(uint32_t deltaMilliseconds) {
    // Check if user closed the window
    for (auto event = sf::Event(); _window->pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            _window->close();
        }
    }

    if (!_gameStarted) {
        _mainMenu->update(*_window);
        _gameStarted = _mainMenu->getStartPressed();
    }
    else _world->update(deltaMilliseconds);
}

void Game::render() {
    _window->clear();
    if (!_gameStarted) {
        _mainMenu->render(*_window);
    }
    else _world->render(*_window);

    _window->display();
}
