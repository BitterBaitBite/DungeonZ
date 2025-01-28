#include "Map/Room.h"

#include <map>
#include <map>

#include "Core/AssetManager.h"
#include "Render/SFMLOrthogonalLayer.h"
#include "SFML/Graphics/RenderWindow.hpp"

Room::~Room() {
    if (_layers.size() > 0) {
        for (auto layer : _layers) {
            delete layer;
        }
        _layers.clear();
    }

    if (_collisionLayers.size() > 0) {
        for (auto collisionLayer : _collisionLayers) {
            delete collisionLayer;
        }
        _collisionLayers.clear();
    }
}


// // TODO load different maps depending on available doors
void Room::init() {
    _map = AssetManager::getInstance()->loadMap("../Data/Maps/DungeonZ_Init.tmx");

    if (_map == nullptr) return;

    for (int i = 0; i < _map->getLayers().size(); i++) {
        if (_map->getLayers()[i]->getType() == tmx::Layer::Type::Tile) {
            _layers.push_back(new MapLayer(*_map, i));
        }
        else if (_map->getLayers()[i]->getType() == tmx::Layer::Type::Object) {
            if (_map->getLayers()[i]->getName() == "Collisions") {
                _collisionLayers.push_back(new ObjectLayer(*_map, i));
            }
        }
    }
}

void Room::update(float deltaMilliseconds) {
    for (MapLayer* layer : _layers) {
        if (layer != nullptr) {
            layer->update(sf::milliseconds(deltaMilliseconds));
        }
    }
}

void Room::render(sf::RenderWindow& window) {
    for (MapLayer* layer : _layers) {
        if (layer != nullptr) {
            window.draw(*layer);
        }
    }
}

std::string Room::toString(bool current) {
    if (current) return "[x]";
    return "[ ]";
}
