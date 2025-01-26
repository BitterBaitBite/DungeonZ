#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

namespace tmx {
    class Map;
}

namespace sf {
    Texture;
}

class AssetManager {
    public:
        static AssetManager* getInstance();

        ~AssetManager();

        sf::Texture* loadTexture(const char* assetPath);
        sf::Texture* getTexture(const char* assetPath) const;

        tmx::Map* loadMap(const char* assetPath);

    private:
        static AssetManager* _instance;

        std::unordered_map<std::string, sf::Texture*> _texturePathToTexture;
        std::unordered_map<std::string, tmx::Map*> _mapPathToMap;
};
