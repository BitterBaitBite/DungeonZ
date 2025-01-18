#include <corecrt_math.h>
#include <Utils/Vector.h>

#include "SFML/System/Vector2.hpp"

sf::Vector2f Vector::normalize(sf::Vector2f v) {
    if (v.x == .0f && v.y == .0f) {
        return v;
    }

    sf::Vector2f normal;

    float magnitude = sqrtf(v.x * v.x + v.y * v.y);
    normal.x = (v.x / magnitude);
    normal.y = (v.y / magnitude);

    return normal;
}

float Vector::magnitude(sf::Vector2f v) {
    if (v.x == .0f && v.y == .0f) {
        return .0f;
    }

    return sqrtf(v.x * v.x + v.y * v.y);
}
