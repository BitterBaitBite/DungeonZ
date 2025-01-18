#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Zombie.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Assets/SpriteSheet.h"
#include "Gameplay/Player.h"

World::~World() {
    delete _player;
    // delete _enemy;
}

bool World::load(sf::RenderWindow* window) {
    constexpr float millisecondsToSeconds = 1 / 1000.f;

    //----------------- PLAYER TEST ---------------------
    // TO DO read from file
    // Player data: (texture path, rows, cols, speed)
    SpriteSheet::SheetDescriptor playerSheetDesc;
    playerSheetDesc.path = "../Data/Images/Player/Warrior_Blue.png";
    playerSheetDesc.rows = 8;
    playerSheetDesc.cols = 6;
    auto playerSpriteSheet = new SpriteSheet();
    playerSpriteSheet->init(playerSheetDesc);

    Player::PlayerDescriptor playerDesc;
    sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());
    playerDesc.texture = playerTexture;
    playerDesc.position = sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2);
    playerDesc.scale = sf::Vector2f(1.f, 1.f);
    playerDesc.speed = { 400.f * millisecondsToSeconds, 400.f * millisecondsToSeconds };
    playerDesc.tileHeight = (playerDesc.texture->getSize().y / playerSpriteSheet->getRows()) * playerDesc.scale.x;
    playerDesc.tileWidth = (playerDesc.texture->getSize().x / playerSpriteSheet->getRows()) * playerDesc.scale.y;

    _player = new Player();
    const bool playerOk = _player->init(playerDesc);
    //--------------------------------------------------

    return playerOk;
}

void World::update(uint32_t deltaMilliseconds) {
    _player->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    _player->render(window);
}
