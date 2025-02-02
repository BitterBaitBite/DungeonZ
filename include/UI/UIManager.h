#pragma once
#include "MouseCursor.h"
#include "PlayerUI.h"
#include "SFML/Graphics/RenderWindow.hpp"


class UIManager {
    public:
        UIManager() = default;
        ~UIManager();

        bool load();
        void update(sf::RenderWindow& window, float deltaTime);
        void render(sf::RenderWindow& window, bool hasGameStarted);
        void reset();

    private:
        sf::Font* _font { nullptr };
        PlayerUI* _playerUI { nullptr };
        MouseCursor* _mouseCursor { nullptr };

        sf::Vector2i _enemyCount { 0, 0 };

    public:
        bool HasPressedRestart() const;
        bool HasFinished() const { return _enemyCount.x == 0; }
};
