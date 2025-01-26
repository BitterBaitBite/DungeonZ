#pragma once

#include <unordered_set>
#include <SFML/Graphics/Texture.hpp>

class Enemy;

class SpawnPool {
    public:
        static SpawnPool* getInstance();

        ~SpawnPool();

        sf::Texture* loadTexture(const char* assetPath);

        sf::Texture* getTexture(const char* assetPath) const;

    private:
        static SpawnPool* _instance;

        std::unordered_set<std::string, Enemy*> _texturePathToTexture;
};
