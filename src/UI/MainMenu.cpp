#include "UI/MainMenu.h"

#include <tmxlite/Types.hpp>

#include "Core/AssetManager.h"
#include "Core/WindowManager.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Utils/Constants.h"

MainMenu::MainMenu() {
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();
    auto buttonSize = sf::Vector2u(TILE_WIDTH * 3, TILE_HEIGHT * 1);
    startButtonCollider = sf::FloatRect(
        windowSize.x / 2 - buttonSize.x / 2,
        windowSize.y - 2 * buttonSize.y,
        buttonSize.x,
        buttonSize.y
    );
}

MainMenu::~MainMenu() {
    if (_layers.size() > 0) {
        for (auto layer : _layers) {
            delete layer;
        }
        _layers.clear();
    }
}

bool MainMenu::load() {
    auto path = "../Data/UI/Menu/MainMenu.tmx";
    _map = AssetManager::getInstance()->loadMap(path);

    if (_map == nullptr) return false;

    for (int i = 0; i < _map->getLayers().size(); i++) {
        if (_map->getLayers()[i]->getType() == tmx::Layer::Type::Tile) {
            _layers.push_back(new MapLayer(*_map, i));
        }
    }

    // Load font
    _font = AssetManager::getInstance()->loadFont("../Data/Fonts/Jacquard12-Regular.ttf");

    return _font != nullptr;
}

void MainMenu::update(sf::RenderWindow& window, uint32_t deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        _startPressed = true;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (startButtonCollider.contains(mousePos.x, mousePos.y)) {
            _startPressed = true;
        }
    }

    for (MapLayer* layer : _layers) {
        if (layer != nullptr) {
            layer->update(sf::milliseconds(deltaTime));
        }
    }
}

void MainMenu::renderTitle(sf::RenderWindow& window) {
    sf::Text title;
    title.setFont(*_font);
    title.setString(GAME_TITLE);
    title.setCharacterSize(100);
    title.setFillColor(sf::Color(200, 55, 25));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2.f);
    title.setPosition(200, 120);

    window.draw(title);
}

void MainMenu::debugButtonCollider(sf::RenderWindow& window) {
    sf::RectangleShape button;
    button.setFillColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(1.f);
    button.setPosition(startButtonCollider.left, startButtonCollider.top);
    button.setSize(sf::Vector2f(startButtonCollider.width, startButtonCollider.height));
    window.draw(button);
}

void MainMenu::renderStartButton(sf::RenderWindow& window) {
    // debugButtonCollider(window);

    sf::Text startButtonText;
    startButtonText.setFont(*_font);
    startButtonText.setString("Start");
    startButtonText.setCharacterSize(startButtonCollider.height / 2);
    startButtonText.setFillColor(sf::Color(200, 55, 25));
    startButtonText.setOutlineColor(sf::Color::Black);
    startButtonText.setOutlineThickness(2.f);
    startButtonText.setPosition(
        startButtonCollider.left + startButtonCollider.width / 2 - startButtonText.getGlobalBounds().width / 2,
        startButtonCollider.top + startButtonCollider.height / 2 - startButtonText.getGlobalBounds().height - 10
    );
    window.draw(startButtonText);
}

void MainMenu::renderBackground(sf::RenderWindow& window) {
    for (MapLayer* layer : _layers) {
        if (layer != nullptr) {
            window.draw(*layer);
        }
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    if (_font == nullptr) return;

    renderBackground(window);

    renderTitle(window);

    renderStartButton(window);
}

void MainMenu::reset() {
    _startPressed = false;
}
