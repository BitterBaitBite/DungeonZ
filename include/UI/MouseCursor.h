#pragma once

#include <tmxlite/Types.hpp>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"


class MouseCursor {
    public:
        MouseCursor(sf::RenderWindow* window);
        ~MouseCursor();

        void update(sf::RenderWindow& window);
        void render(sf::RenderWindow& window);

    private:
        bool _isSelect { false };
        sf::Vector2i _pos { 0, 0 };
        sf::Sprite _activeSprite;
        sf::Sprite* _defaultCursor { nullptr };
        sf::Sprite* _selectCursor { nullptr };

    public:
        bool isSelect() const { return _isSelect; }
        void setIsSelect(bool isSelect) { _isSelect = isSelect; }
};
