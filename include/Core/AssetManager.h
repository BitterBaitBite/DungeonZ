#pragma once

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

#include "SFML/Graphics/Font.hpp"

namespace tmx {
    class Map;
}

class AssetManager {
    public:
        static AssetManager* getInstance();

        ~AssetManager();

        sf::Texture* loadTexture(const char* assetPath);
        sf::Texture* getTexture(const char* assetPath) const;

        sf::Font* loadFont(const char* assetPath);

        tmx::Map* loadMap(const char* assetPath);

    private:
        static AssetManager* _instance;

        std::unordered_map<std::string, sf::Texture*> _texturePathToTexture;
        std::unordered_map<std::string, sf::Font*> _fontPathToFont;
        std::unordered_map<std::string, tmx::Map*> _mapPathToMap;
};
