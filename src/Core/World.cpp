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

void World::reset() {
    Player::PlayerInfo playerInfo;
    getPlayerInfo(playerInfo);
    Player::getInstance()->reset(playerInfo);

    _currentDungeon->reset();
}

void World::getPlayerSpriteInfo(SpriteSheet*& playerSpriteSheet, sf::Texture*& playerTexture) {
    SpriteSheet::SpriteSheetInfo playerSheetDesc;
    playerSheetDesc.path = "../Data/Images/Player/Warrior_Blue.png";
    playerSheetDesc.rows = 8;
    playerSheetDesc.cols = 6;
    playerSpriteSheet = new SpriteSheet(playerSheetDesc);
    playerTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());
}

void World::getPlayerFireSpriteInfo(SpriteSheet*& fireSpriteSheet, sf::Texture*& fireTexture) {
    SpriteSheet::SpriteSheetInfo fireSheetDesc;
    fireSheetDesc.path = "../Data/Images/Effects/Fire.png";
    fireSheetDesc.rows = 1;
    fireSheetDesc.cols = 7;
    fireSpriteSheet = new SpriteSheet(fireSheetDesc);
    fireTexture = AssetManager::getInstance()->loadTexture(fireSpriteSheet->getPath());
}

void World::setPlayerInfo(SpriteSheet* playerSpriteSheet, sf::Texture* playerTexture, SpriteSheet* fireSpriteSheet,
                          sf::Texture* fireTexture, Player::PlayerInfo& playerInfo) {
    playerInfo.texture = playerTexture;
    playerInfo.fireTexture = fireTexture;
    playerInfo.position = WindowManager::getInstance()->getWindowCenter();
    playerInfo.speed = { 400.f * MILLISECONDS_TO_SECONDS, 400.f * MILLISECONDS_TO_SECONDS };
    playerInfo.spriteHeight = playerInfo.texture->getSize().y / playerSpriteSheet->getRows() * playerInfo.scale.y;
    playerInfo.spriteWidth = playerInfo.texture->getSize().x / playerSpriteSheet->getCols() * playerInfo.scale.x;
    playerInfo.fireSpriteHeight = playerInfo.fireTexture->getSize().y / fireSpriteSheet->getRows() * playerInfo.scale.y;
    playerInfo.fireSpriteWidth = playerInfo.fireTexture->getSize().x / fireSpriteSheet->getCols() * playerInfo.scale.x;
    playerInfo.maxHealth = 10;
}

void World::getPlayerInfo(Player::PlayerInfo& playerInfo) {
    SpriteSheet* playerSpriteSheet;
    sf::Texture* playerTexture;
    getPlayerSpriteInfo(playerSpriteSheet, playerTexture);

    SpriteSheet* fireSpriteSheet;
    sf::Texture* fireTexture;
    getPlayerFireSpriteInfo(fireSpriteSheet, fireTexture);

    setPlayerInfo(playerSpriteSheet, playerTexture, fireSpriteSheet, fireTexture, playerInfo);
}

bool World::loadPlayer() {
    Player::PlayerInfo playerInfo;
    getPlayerInfo(playerInfo);

    const bool playerOk = Player::getInstance()->init(playerInfo);

    return playerOk;
}
