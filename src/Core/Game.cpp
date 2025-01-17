#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

bool Game::init(GameCreateInfo& createInfo) {
    assert(_window == nullptr && _world == nullptr, "Game is already initialized, we are about to leak memory");

    _window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
    _window->setFramerateLimit(createInfo.frameRateLimit);
    _world = new World();
    const bool loadOk = _world->load(_window);

    return loadOk;
}

Game::~Game() {
    delete _world;
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

    // Update scene here
    _world->update(deltaMilliseconds);
}

void Game::render() {
    _window->clear();

    _world->render(*_window);

    _window->display();
}
