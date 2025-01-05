#include <corecrt_math.h>
#include <Utils/Vector.h>

#include "SFML/System/Vector2.hpp"

static sf::Vector2f normalize(sf::Vector2f v) {
    sf::Vector2f normal;

    float magnitude = sqrtf(v.x * v.x + v.y * v.y);
    normal.x = (v.x / magnitude);
    normal.y = (v.y / magnitude);

    return normal;
}
