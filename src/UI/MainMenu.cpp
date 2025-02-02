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
    auto buttonSize = sf::Vector2u(windowSize.x / 3, windowSize.y / 6);
    startButtonCollider = sf::FloatRect(
        windowSize.x / 2 - buttonSize.x / 2,
        windowSize.y - 2 * buttonSize.y,
        buttonSize.x,
        buttonSize.y
    );
}

bool MainMenu::load() {
    // Load font
    _font = AssetManager::getInstance()->loadFont("../Data/Fonts/Jacquard12-Regular.ttf");

    return _font != nullptr;
}

void MainMenu::update(sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        _startPressed = true;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (startButtonCollider.contains(mousePos.x, mousePos.y)) {
            _startPressed = true;
        }
    }
}

void MainMenu::renderTitle(sf::RenderWindow& window) {
    sf::Text title;
    title.setFont(*_font);
    title.setString(GAME_TITLE);
    title.setCharacterSize(100);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 200);

    window.draw(title);
}

void MainMenu::renderStartButton(sf::RenderWindow& window) {
    sf::RectangleShape button;
    button.setFillColor(sf::Color(180, 120, 100));
    button.setPosition(startButtonCollider.left, startButtonCollider.top);
    button.setSize({ startButtonCollider.width, startButtonCollider.height });
    window.draw(button);

    sf::Text startButtonText;
    startButtonText.setFont(*_font);
    startButtonText.setString("Start");
    startButtonText.setCharacterSize(startButtonCollider.height / 2);
    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setPosition(
        window.getSize().x / 2 - startButtonText.getGlobalBounds().width / 2,
        startButtonCollider.top + startButtonText.getGlobalBounds().height / 2
    );
    window.draw(startButtonText);
}

void MainMenu::render(sf::RenderWindow& window) {
    if (_font == nullptr) return;

    sf::CircleShape mouseCircle;
    mouseCircle.setFillColor(sf::Color(200, 150, 100));
    mouseCircle.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    mouseCircle.setRadius(30.f);
    window.draw(mouseCircle);

    renderTitle(window);

    renderStartButton(window);
}
