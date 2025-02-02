#pragma once
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

class PlayerUI {
    public:
        PlayerUI();
        ~PlayerUI() = default;

        bool load();
        void update(float deltaMilliseconds);
        void render(sf::RenderWindow& window);
        void reset();

        void setDeathText(float& textPosX, float& textPosY);
        void setRestartText(float& textPosX, float textPosY);
        bool HasPressedRestart() const { return _hasPressedRestart; }

    private:
        sf::Font* _font { nullptr };
        sf::Text _deathText;
        sf::Text _restartText;

        const float RESTART_TEXT_DELAY { 2.f };
        sf::Clock _restartTextClock;
        bool _hasDelayStarted { false };
        bool _restartTextAlpha { false };

        bool _isDead { false };
        const sf::Color _healthColor = sf::Color::Green;
        const sf::Color _burningColor = sf::Color(200, 120, 100);
        sf::RectangleShape _currentHealthUI;
        sf::RectangleShape _maxHealthUI;

        bool _hasPressedRestart { false };
};
