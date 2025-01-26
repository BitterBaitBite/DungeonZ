#include <map>
#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Villager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>

#include "Assets/SpriteSheet.h"
#include "Gameplay/Player.h"
#include "Render/SFMLOrthogonalLayer.h"

World::~World() {
    delete _player;
    delete _currentDungeon;
    // delete _map;
    // delete _level01;
    // delete _enemy;
}

bool World::load(sf::RenderWindow* window) {
    constexpr float millisecondsToSeconds = 1 / 1000.f;
    auto windowCenterPosition = sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f);

    // Tmxlite map test
    // _map = new tmx::Map();
    // _map->load("../Data/Maps/DungeonZ_Init.tmx");
    // for (int i = 0; i < _map->getLayers().size(); i++) {
    //     if (_map->getLayers().at(i)->getType() == tmx::Layer::Type::Tile) {
    //         _mapLayers.push_back(new MapLayer(*_map, i));
    //     }
    // }
    _currentDungeon = new Dungeon();

    //----------------- PLAYER TEST ---------------------
    // TO DO read from file
    // Player texture data: (texture path, rows, cols)
    SpriteSheet::SheetDescriptor playerSheetDesc;
    playerSheetDesc.path = "../Data/Images/Player/Warrior_Blue.png";
    playerSheetDesc.rows = 8;
    playerSheetDesc.cols = 6;
    auto playerSpriteSheet = new SpriteSheet(playerSheetDesc);

    // Player object data: (texture
    Player::PlayerDescriptor playerDesc;
    sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());
    playerDesc.texture = playerTexture;
    playerDesc.position = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
    playerDesc.scale = sf::Vector2f(1.f, 1.f);
    playerDesc.speed = { 400.f * millisecondsToSeconds, 400.f * millisecondsToSeconds };
    playerDesc.spriteHeight = (playerDesc.texture->getSize().y / playerSpriteSheet->getRows()) * playerDesc.scale.y;
    playerDesc.spriteWidth = (playerDesc.texture->getSize().x / playerSpriteSheet->getCols()) * playerDesc.scale.x;

    _player = new Player();
    const bool playerOk = _player->init(playerDesc);
    //--------------------------------------------------

    return playerOk;
}

void World::update(uint32_t deltaMilliseconds) {
    // Map test
    // for (MapLayer* layer : _mapLayers) {
    //     if (layer != nullptr) {
    //         layer->update(sf::milliseconds(deltaMilliseconds));
    //     }
    // }

    // Dungeon
    _currentDungeon->update(deltaMilliseconds);

    // Player
    _player->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    // Map test
    // for (MapLayer* layer : _mapLayers) {
    //     if (layer != nullptr) {
    //         window.draw(*layer);
    //     }
    // }

    // Dungeon
    _currentDungeon->render(window);

    // Player
    _player->render(window);
}
