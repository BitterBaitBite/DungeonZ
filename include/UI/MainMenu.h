#pragma once
#include "SFML/Graphics/Font.hpp"

namespace sf {
    class RenderWindow;
}

class MainMenu {
    public:
        MainMenu();
        ~MainMenu() = default;

        bool load();
        void update(sf::RenderWindow& window);
        void render(sf::RenderWindow& window);

        bool getStartPressed() const { return _startPressed; }

    private:
        sf::Font* _font { nullptr };
        bool _startPressed { false };
        sf::FloatRect startButtonCollider { 0.f, 0.f, 0.f, 0.f };

        void renderTitle(sf::RenderWindow& window);
        void renderStartButton(sf::RenderWindow& window);
};
