#pragma once
#include "SFML/System/Vector2.hpp"

class Vector {
    public:
        static sf::Vector2f normalize(const sf::Vector2f& vector);
        static float magnitude(const sf::Vector2f& vector);
        static float distance(const sf::Vector2f& a, const sf::Vector2f& b);
        static float heuristicDistance(int posX1, int posY1, int posX2, int posY2);
        static float heuristicDistance(const sf::Vector2f& a, const sf::Vector2f& b);
        static std::vector<sf::Vector2i> getPoints(sf::Vector2i start, sf::Vector2i end);
};
