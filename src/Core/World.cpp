#include <map>
#include <Core/AssetManager.h>
#include <Core/World.h>
#include <tmxlite/Map.hpp>

#include "Assets/SpriteSheet.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Gameplay/Player.h"
#include "Gameplay/Villager.h"
#include "Render/SFMLOrthogonalLayer.h"

World::~World() {
    delete _currentDungeon;
}


bool World::load() {
    // Load current dungeon (level)
    _currentDungeon = DungeonManager::getInstance()->loadDungeon();

    // Load player
    return loadPlayer();
}

void World::update(uint32_t deltaMilliseconds) {
    // Dungeon
    _currentDungeon->update(deltaMilliseconds);

    // Player
    Player::getInstance()->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    // Dungeon
    _currentDungeon->render(window);

    // Player
    Player::getInstance()->render(window);
}

bool World::loadPlayer() {
    SpriteSheet::SheetDescriptor playerSheetDesc;
    playerSheetDesc.path = "../Data/Images/Player/Warrior_Blue.png";
    playerSheetDesc.rows = 8;
    playerSheetDesc.cols = 6;
    auto playerSpriteSheet = new SpriteSheet(playerSheetDesc);
    sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());

    SpriteSheet::SheetDescriptor fireSheetDesc;
    fireSheetDesc.path = "../Data/Images/Effects/Fire.png";
    fireSheetDesc.rows = 1;
    fireSheetDesc.cols = 7;
    auto fireSpriteSheet = new SpriteSheet(fireSheetDesc);
    sf::Texture* fireTexture = AssetManager::getInstance()->loadTexture(fireSpriteSheet->getPath());

    Player::PlayerInfo playerDesc;
    playerDesc.texture = playerTexture;
    playerDesc.fireTexture = fireTexture;
    playerDesc.position = WindowManager::getInstance()->getWindowCenter();
    playerDesc.speed = { 400.f * MILLISECONDS_TO_SECONDS, 400.f * MILLISECONDS_TO_SECONDS };
    playerDesc.spriteHeight = playerDesc.texture->getSize().y / playerSpriteSheet->getRows() * playerDesc.scale.y;
    playerDesc.spriteWidth = playerDesc.texture->getSize().x / playerSpriteSheet->getCols() * playerDesc.scale.x;
    playerDesc.fireSpriteHeight = playerDesc.fireTexture->getSize().y / fireSpriteSheet->getRows() * playerDesc.scale.y;
    playerDesc.fireSpriteWidth = playerDesc.fireTexture->getSize().x / fireSpriteSheet->getCols() * playerDesc.scale.x;
    playerDesc.maxHealth = 10;

    const bool playerOk = Player::getInstance()->init(playerDesc);

    return playerOk;
}
