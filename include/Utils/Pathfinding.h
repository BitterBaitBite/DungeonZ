#pragma once
#include <vector>

#include "Constants.h"
#include "SFML/System/Vector2.hpp"

class Pathfinding {
    public:
        struct Node {
            // Position coordinates
            int x, y;

            // gCost = path length
            // hCost = distance from adjacent position to goal
            // fCost = gCost + hCost (total cost)
            float gCost, hCost, fCost;

            // Parent node / origin node
            Node* parent;

            // Order by total cost
            bool operator>(const Node& other) const {
                return fCost > other.fCost;
            }
        };

        static std::vector<sf::Vector2i> Pathfinding::getPath(
            sf::Vector2i start,
            sf::Vector2i goal,
            const std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& grid);
};
