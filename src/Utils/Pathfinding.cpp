#include "Utils/Pathfinding.h"

#include <array>
# include <cmath>
# include <queue>
# include <unordered_map>
# include <vector>

#include "Core/AssetManager.h"
#include "Utils/Vector.h"


std::vector<sf::Vector2i> Pathfinding::getPath(
    sf::Vector2i start,
    sf::Vector2i goal,
    const std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& grid
) {
    std::unordered_map<int, Node> allNodes;

    // Nodes to check ordered by fCost (total cost)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

    // Gets a unique index for any two-dimensional coordinates of a node
    auto nodeIndex = [](int x, int y) { return y * BACKGROUND_ROW_SIZE + x; };

    // Sets the node to start
    Node startNode;
    {
        startNode.x = start.x;
        startNode.y = start.y;
        startNode.gCost = 0;
        startNode.hCost = Vector::heuristicDistance(start.x, start.y, goal.x, goal.y);
        startNode.fCost = 0;
        startNode.parent = nullptr;
    }
    openSet.push(startNode);
    allNodes[nodeIndex(start.x, start.y)] = startNode;

    // While there's nodes to check
    while (!openSet.empty()) {
        // Extracts the top node from the search queue (the one with least fCost)
        Node current = openSet.top();
        openSet.pop();

        // If the node is the goal, gets the path backwards by node parent and reverses it
        if (current.x == goal.x && current.y == goal.y) {
            std::vector<sf::Vector2i> path;
            while (current.parent) {
                path.push_back({ current.x, current.y });
                current = *current.parent;
            }
            std::reverse(path.begin(), path.end());

            return path;
        }

        // Creates a vector with the adjacent node coordinates
        std::vector<sf::Vector2i> adjacents = {
            { current.x + 1, current.y },
            { current.x - 1, current.y },
            { current.x, current.y + 1 },
            { current.x, current.y - 1 }
        };

        // Checks all adjacent nodes
        for (const auto& adjacent : adjacents) {
            // Ignores collision or non existent nodes
            if (grid[adjacent.y][adjacent.x]) continue;

            // Updates the costs (gCost = path length / hCost = distance from adjacent position to goal / fCost = gCost + hCost (total cost)
            float gCost = current.gCost + 1;
            float hCost = Vector::heuristicDistance(adjacent.x, adjacent.y, goal.x, goal.y);
            float fCost = gCost + hCost;

            // Continues on adjacent node if already checked and total cost is less or equal
            if (allNodes.count(nodeIndex(adjacent.x, adjacent.y)) // Counts nodes by index
                && allNodes[nodeIndex(adjacent.x, adjacent.y)].fCost <= fCost)
                continue;

            Node adjacentNode = {
                adjacent.x,
                adjacent.y,
                gCost,
                hCost,
                fCost,
                &allNodes[nodeIndex(current.x, current.y)]
            };

            openSet.push(adjacentNode);
            allNodes[nodeIndex(adjacent.x, adjacent.y)] = adjacentNode;
        }
    }

    return {};
}
