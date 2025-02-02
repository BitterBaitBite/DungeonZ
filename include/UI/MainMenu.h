#pragma once
#include "Core/AssetManager.h"
#include "Render/SFMLOrthogonalLayer.h"
#include "SFML/Graphics/Font.hpp"

namespace sf {
    class RenderWindow;
}

class MainMenu {
    public:
        MainMenu();
        ~MainMenu();

        bool load();
        void update(sf::RenderWindow& window, uint32_t deltaTime);
        void render(sf::RenderWindow& window);
        void reset();

        bool getStartPressed() const { return _startPressed; }

    private:
        tmx::Map* _map { nullptr };
        std::vector<MapLayer*> _layers;
        sf::Font* _font { nullptr };
        bool _startPressed { false };
        sf::FloatRect startButtonCollider { 0.f, 0.f, 0.f, 0.f };

        void renderTitle(sf::RenderWindow& window);
        void debugButtonCollider(sf::RenderWindow& window);
        void renderStartButton(sf::RenderWindow& window);
        void renderBackground(sf::RenderWindow& window);
};
