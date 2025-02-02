#include "Map/Room.h"

#include <bitset>

#include "Core/AssetManager.h"
#include "Enums/DirectionEnum.h"
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

void Room::update(uint32_t deltaTime) {
    for (MapLayer* layer : _layers) {
        if (layer != nullptr) {
            layer->update(sf::milliseconds(deltaTime));
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


void Room::init(std::vector<DirectionEnum> adjacentRooms = {
    DirectionEnum::Right,
    DirectionEnum::Down,
    DirectionEnum::Up,
    DirectionEnum::Left
}) {
    const char* path = getRoomMapPath(adjacentRooms);
    _map = AssetManager::getInstance()->loadMap(path);

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

    initObjectColliders();
}

std::bitset<4> Room::getDirectionsMask(std::vector<DirectionEnum> directions) {
    std::bitset<4> roomFlags = 0;
    for (auto direction : directions) {
        switch (direction) {
            case DirectionEnum::Right:
                roomFlags |= 1 << 0;
                break;

            case DirectionEnum::Down:
                roomFlags |= 1 << 1;
                break;

            case DirectionEnum::Up:
                roomFlags |= 1 << 2;
                break;

            case DirectionEnum::Left:
                roomFlags |= 1 << 3;
                break;
        }
    }

    return roomFlags;
}

const char* Room::getRoomMapPath(std::vector<DirectionEnum> availableRooms) {
    auto path = "../Data/Maps/DungeonZ_Init.tmx";

    if (availableRooms.size() == 0) return path;

    std::bitset<4> roomMask = getDirectionsMask(availableRooms);
    switch (roomMask.to_ulong()) {
        // From 0000 bitset
        // Left >> Up >> Down >> Right

        case 0b0001:
            path = "../Data/Maps/OneWay/RightRoom_01.tmx";
            break;

        case 0b0010:
            path = "../Data/Maps/OneWay/DownRoom_01.tmx";
            break;

        case 0b0011:
            path = "../Data/Maps/TwoWays/RightDownRoom.tmx";
            break;

        case 0b0100:
            path = "../Data/Maps/OneWay/UpRoom_01.tmx";
            break;

        case 0b0101:
            path = "../Data/Maps/TwoWays/UpRightRoom.tmx";
            break;

        case 0b0110:
            path = "../Data/Maps/TwoWays/UpDownRoom.tmx";
            break;

        case 0b0111:
            path = "../Data/Maps/ThreeWays/UpRightDownRoom.tmx";
            break;

        case 0b1000:
            path = "../Data/Maps/OneWay/LeftRoom_01.tmx";
            break;

        case 0b1001:
            path = "../Data/Maps/TwoWays/RightLeftRoom.tmx";
            break;

        case 0b1010:
            path = "../Data/Maps/TwoWays/DownLeftRoom.tmx";
            break;

        case 0b1011:
            path = "../Data/Maps/ThreeWays/RightDownLeftRoom.tmx";
            break;

        case 0b1100:
            path = "../Data/Maps/TwoWays/UpLeftRoom.tmx";
            break;

        case 0b1101:
            path = "../Data/Maps/ThreeWays/UpRightLeftRoom.tmx";
            break;

        case 0b1110:
            path = "../Data/Maps/ThreeWays/UpDownLeftRoom.tmx";
            break;

        case 0b1111:
            path = "../Data/Maps/CrossRooms/CrossRoom_01.tmx";
            break;

        case 0:
        default:
            break;
    }

    return path;
}

void Room::initObjectColliders() {
    std::vector<sf::Shape*> shapes;
    for (ObjectLayer* layer : _collisionLayers) {
        if (layer != nullptr) {
            shapes = layer->getShapes();

            for (sf::Shape* shape : shapes) {
                if (shape != nullptr) {
                    _collisionRects.push_back(shape->getGlobalBounds());
                }
            }
        }
    }
}

std::string Room::toString(bool current) {
    if (current) return "[x]";
    return "[ ]";
}
