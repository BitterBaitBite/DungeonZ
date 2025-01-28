#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

enum class FaceDirection;

class CollisionManager {
    public:
        static CollisionManager* getInstance();

        ~CollisionManager() = default;

    private:
        static CollisionManager* _instance;

    public:
        bool isOffScreen(sf::FloatRect rect);
        FaceDirection getWindowCollision(sf::FloatRect rect);
};
