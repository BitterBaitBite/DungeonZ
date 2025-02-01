#include <map>
#include <Core/AssetManager.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>

#include "Assets/SpriteSheet.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Gameplay/Player.h"
#include "Gameplay/Villager.h"
#include "Render/SFMLOrthogonalLayer.h"

World::~World() {
    delete _enemyTest;
    delete _player;
    delete _currentDungeon;
}

bool World::load() {
    sf::RenderWindow* window = WindowManager::getInstance()->loadWindow();
    auto windowCenterPosition = sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f);


    // Load current dungeon (level)
    _currentDungeon = DungeonManager::getInstance()->loadDungeon();

    // //----------------- PLAYER TEST ---------------------
    // // TO DO read from file
    // Player texture data: (texture path, rows, cols)
    SpriteSheet::SheetDescriptor playerSheetDesc;
    playerSheetDesc.path = "../Data/Images/Player/Warrior_Blue.png";
    playerSheetDesc.rows = 8;
    playerSheetDesc.cols = 6;
    auto playerSpriteSheet = new SpriteSheet(playerSheetDesc);

    // Player object data: (texture
    Player::PlayerInfo playerDesc;
    sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());
    playerDesc.texture = playerTexture;
    playerDesc.position = windowCenterPosition;
    playerDesc.speed = { 400.f * MILLISECONDS_TO_SECONDS, 400.f * MILLISECONDS_TO_SECONDS };
    playerDesc.spriteHeight = (playerDesc.texture->getSize().y / playerSpriteSheet->getRows()) * playerDesc.scale.y;
    playerDesc.spriteWidth = (playerDesc.texture->getSize().x / playerSpriteSheet->getCols()) * playerDesc.scale.x;
    playerDesc.maxHealth = 10;

    const bool playerOk = Player::getInstance()->init(playerDesc);
    //--------------------------------------------------

    // Enemy test --------------------------->
    SpriteSheet::SheetDescriptor enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Pawn_Red.png";
    enemySheetDesc.rows = 6;
    enemySheetDesc.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    Villager::VillagerInfo villagerInfo;
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    villagerInfo.texture = enemyTexture;
    villagerInfo.speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    villagerInfo.position = { windowCenterPosition.x - TILE_WIDTH * 2, windowCenterPosition.y };
    villagerInfo.spriteHeight = villagerInfo.texture->getSize().y / enemySpriteSheet->getRows() * villagerInfo.scale.y;
    villagerInfo.spriteWidth = villagerInfo.texture->getSize().x / enemySpriteSheet->getCols() * villagerInfo.scale.x;
    villagerInfo.maxHealth = 3;
    villagerInfo.attackDamage = 2;
    _enemyTest = new Villager();
    bool enemyOk = _enemyTest->init(&villagerInfo);

    // <------------------------ End enemy test


    return enemyOk && playerOk;
    // return playerOk;
}

void World::update(uint32_t deltaMilliseconds) {
    // Dungeon
    _currentDungeon->update(deltaMilliseconds);

    // Player
    Player::getInstance()->update(deltaMilliseconds);

    _enemyTest->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    // Dungeon
    _currentDungeon->render(window);

    // Player
    Player::getInstance()->render(window);

    _enemyTest->render(window);
}
