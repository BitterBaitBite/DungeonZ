#pragma once

#include <Gameplay/Enemy.h>

#include "SFML/Graphics/VertexArray.hpp"
#include "Utils/Constants.h"

class Villager : public Enemy {
    public:
        struct VillagerInfo : EnemyInfo {
            int attackDamage { 1 };
        };

        bool init(EnemyInfo* enemyInfo) override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    private:
        // Animations
        const int MAX_IDLE_TILES = 6;
        const int MAX_MOVEMENT_TILES = 6;
        const int MAX_ATTACK_TILES = 6;

        int GetMaxIdleTiles() const override { return MAX_IDLE_TILES; }
        int GetMaxMovementTiles() const override { return MAX_MOVEMENT_TILES; }

        // Attack
        const float TARGET_RANGE = 200.0f;
        bool _hasCloseTarget { false };
        int _attackDamage { 1 };
        sf::FloatRect _attackCollider { 0.f, 0.f, 0.f, 0.f };

        void calcTargetPosition() override;

        // Render
        void debugSprite(sf::RenderWindow& window) override;

        // Movement
        std::vector<sf::Vector2i> _pathToTarget;
        sf::VertexArray _pathLine;
        size_t _currentTarget = 0;
        sf::Clock _pathClock;
        void move(float deltaMilliseconds) override;
        void getMoveDirection() override;

        // Pathfinding
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

        std::vector<sf::Vector2i> getPath(
            sf::Vector2i start,
            sf::Vector2i goal,
            std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& grid
        );
};
