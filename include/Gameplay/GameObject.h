#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderWindow;
}

class GameObject {
    public:
        GameObject() = default;
        GameObject(float x, float y) : _position { x, y } {}
        virtual ~GameObject() = default;

        // Getters
        const sf::Vector2f& getPosition() const { return _position; }
        sf::Vector2f& getPosition() { return _position; }

        // Setters
        void setPosition(const sf::Vector2f& newPosition) { _position = newPosition; }

        // Methods
        virtual void update(float delta) = 0;
        virtual void render(sf::RenderWindow& window) = 0;

    protected:
        sf::Vector2f _position { .0f, .0f };
};
