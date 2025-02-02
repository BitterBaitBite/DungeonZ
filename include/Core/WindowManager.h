#pragma once
#include "SFML/Graphics/RenderWindow.hpp"

class WindowManager {
    public:
        static WindowManager* getInstance();

        ~WindowManager();

    private:
        static WindowManager* _instance;
        sf::RenderWindow* _window { nullptr };

    public:
        sf::RenderWindow* loadWindow();
        sf::RenderWindow* loadWindow(sf::VideoMode videoMode, std::string title, uint32_t frameRateLimit);

        sf::Vector2u getWindowSize() const;

        sf::Vector2f getWindowCenter() const {
            return sf::Vector2f(_window->getSize().x / 2.f, _window->getSize().y / 2.f);
        }
};
