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
        PlayerUI* _playerUI { nullptr };
        MouseCursor* _mouseCursor { nullptr };

    public:
        bool HasPressedRestart() const;
};
