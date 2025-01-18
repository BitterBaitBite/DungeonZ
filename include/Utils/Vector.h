#pragma once
#include "SFML/System/Vector2.hpp"

class Vector {
    public:
        static sf::Vector2f normalize(sf::Vector2f vector);
        static float magnitude(sf::Vector2f vector);
};
