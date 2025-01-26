#include <Core/AssetManager.h>
#include <tmxlite/Map.hpp>

AssetManager* AssetManager::_instance { nullptr };

AssetManager* AssetManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new AssetManager();
    }
    return _instance;
}

AssetManager::~AssetManager() {
    for (auto pair : _texturePathToTexture) {
        delete pair.second;
    }
}

sf::Texture* AssetManager::loadTexture(const char* assetPath) {
    auto pair = _texturePathToTexture.find(assetPath);
    if (pair != _texturePathToTexture.end()) {
        return pair->second; // Returns the existing texture
    }

    auto newTexture = new sf::Texture();
    const bool loadOk = newTexture->loadFromFile(assetPath);
    if (!loadOk) {
        delete newTexture;
        return nullptr; // Returns a null pointer if load error
    }

    _texturePathToTexture[assetPath] = newTexture;
    return newTexture; // Returns the new texture
}

// Returns the specified texture by name (key)
sf::Texture* AssetManager::getTexture(const char* assetPath) const {
    const auto it = _texturePathToTexture.find(assetPath);

    return (it != _texturePathToTexture.end()) ? it->second : nullptr;
}

tmx::Map* AssetManager::loadMap(const char* assetPath) {
    auto pair = _mapPathToMap.find(assetPath);
    if (pair != _mapPathToMap.end()) {
        return pair->second; // Returns the existing map
    }

    auto newMap = new tmx::Map();
    const bool loadOk = newMap->load(assetPath);
    if (!loadOk) {
        delete newMap;
        return nullptr; // Returns a null pointer if load error
    }

    _mapPathToMap[assetPath] = newMap;
    return newMap; // Returns the new map
}
