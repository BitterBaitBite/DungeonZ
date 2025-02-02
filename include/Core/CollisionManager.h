#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Enemy;
class IDamageable;
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
        bool hasPlayerCollision(sf::FloatRect rect);
        std::vector<Enemy*> getEnemyCollisions(sf::FloatRect rect);
};
