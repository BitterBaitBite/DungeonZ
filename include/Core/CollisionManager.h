#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

enum class DirectionEnum;

class CollisionManager {
    public:
        static CollisionManager* getInstance();

        ~CollisionManager() = default;

    private:
        static CollisionManager* _instance;

    public:
        bool isOffScreen(sf::FloatRect rect);
        bool hasObjectCollision(sf::FloatRect rect);
        bool hasObjectCollision(sf::FloatRect rect, std::vector<DirectionEnum>& directions);
        bool hasEnemyCollision(sf::FloatRect rect);
};
