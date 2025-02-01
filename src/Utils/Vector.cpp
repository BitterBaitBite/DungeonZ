#include <corecrt_math.h>
#include <valarray>
#include <vector>
#include <Utils/Vector.h>

#include "SFML/System/Vector2.hpp"

sf::Vector2f Vector::normalize(const sf::Vector2f& vector) {
    if (vector.x == .0f && vector.y == .0f) {
        return vector;
    }

    // float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);
    float magnitude = Vector::magnitude(vector);
    sf::Vector2f normal = { vector.x / magnitude, vector.y / magnitude };

    return normal;
}

float Vector::magnitude(const sf::Vector2f& vector) {
    if (vector.x == .0f && vector.y == .0f) {
        return .0f;
    }

    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

float Vector::distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    sf::Vector2f resultVector = { b.x - a.x, b.y - a.y };

    return magnitude(resultVector);
}

float Vector::heuristicDistance(int posX1, int posY1, int posX2, int posY2) {
    return std::abs(posX1 - posX2) + std::abs(posY1 - posY2);
}

float Vector::heuristicDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<sf::Vector2i> Vector::getPoints(sf::Vector2i start, sf::Vector2i end) {
    std::vector<sf::Vector2i> points;

    int distanceX = abs(end.x - start.x);
    int distanceY = abs(end.y - start.y);
    int signX = (start.x < end.x) ? 1 : -1;
    int signY = (start.y < end.y) ? 1 : -1;
    int err = distanceX - distanceY;

    while (true) {
        points.push_back(start);
        if (start == end) break;

        int e2 = err * 2;
        if (e2 > -distanceY) {
            err -= distanceY;
            start.x += signX;
        }
        if (e2 < distanceX) {
            err += distanceX;
            start.y += signY;
        }
    }

    return points;
}
