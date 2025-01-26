#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Villager.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Assets/SpriteSheet.h"
#include "Gameplay/Player.h"
#include "Map/Background.h"
#include "Map/Room.h"

World::~World() {
    delete _level01;
    delete _player;
    // delete _enemy;
}

bool World::load(sf::RenderWindow* window) {
    constexpr float millisecondsToSeconds = 1 / 1000.f;
    auto windowCenterPosition = sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f);

    // Background
    _background = new Background(
        sf::Vector2f(
            windowCenterPosition.x - BACKGROUND_ROW_SIZE * TILE_WIDTH / 2.f,
            windowCenterPosition.y - BACKGROUND_COL_SIZE * TILE_WIDTH / 2.f
        )
    );

    // Room
    _level01 = new Room(
        sf::Vector2f(
            windowCenterPosition.x - ROOM_ROW_SIZE * TILE_WIDTH / 2.f,
            windowCenterPosition.y - ROOM_COL_SIZE * TILE_WIDTH / 2.f
        )
    );

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
    _level01->update(deltaMilliseconds);
    _player->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    _background->render(window);
    _level01->render(window);
    _player->render(window);
}
