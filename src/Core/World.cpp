#include <map>
#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Villager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>

#include "Assets/SpriteSheet.h"
#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Gameplay/Player.h"
#include "Render/SFMLOrthogonalLayer.h"

World::~World() {
    delete _player;
    delete _currentDungeon;
    // delete _map;
    // delete _level01;
    // delete _enemy;
}

bool World::load() {
    sf::RenderWindow* window = WindowManager::getInstance()->loadWindow();
    auto windowCenterPosition = sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f);

    constexpr float millisecondsToSeconds = 1 / 1000.f;

    // Load current dungeon (level)
    _currentDungeon = DungeonManager::getInstance()->loadDungeon();

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
    playerDesc.position = windowCenterPosition;
    playerDesc.scale = sf::Vector2f(1.f, 1.f);
    playerDesc.speed = { 400.f * millisecondsToSeconds, 400.f * millisecondsToSeconds };
    playerDesc.spriteHeight = (playerDesc.texture->getSize().y / playerSpriteSheet->getRows()) * playerDesc.scale.y;
    playerDesc.spriteWidth = (playerDesc.texture->getSize().x / playerSpriteSheet->getCols()) * playerDesc.scale.x;
    playerDesc.maxHealth = 10;

    _player = new Player();
    const bool playerOk = _player->init(playerDesc);

    //--------------------------------------------------

    SpriteSheet::SheetDescriptor enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Pawn_Purple.png";
    enemySheetDesc.rows = 8;
    enemySheetDesc.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    Enemy::EnemyDescriptor enemyDesc;
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(playerSpriteSheet->getPath());
    enemyDesc.texture = enemyTexture;
    enemyDesc.position = { windowCenterPosition.x + 2 * TILE_WIDTH, windowCenterPosition.y };
    enemyDesc.scale = sf::Vector2f(1.f, 1.f);
    enemyDesc.speed = { 400.f * millisecondsToSeconds, 400.f * millisecondsToSeconds };
    enemyDesc.spriteHeight = (enemyDesc.texture->getSize().y / playerSpriteSheet->getRows()) * enemyDesc.scale.y;
    enemyDesc.spriteWidth = (enemyDesc.texture->getSize().x / playerSpriteSheet->getCols()) * enemyDesc.scale.x;
    enemyDesc.maxHealth = 10;

    _enemy = new Enemy();
    const bool enemyOk = _enemy->init(playerDesc);

    return playerOk && enemyOk;
}

void World::update(uint32_t deltaMilliseconds) {
    // Dungeon
    _currentDungeon->update(deltaMilliseconds);

    // Player
    _player->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window) {
    // Dungeon
    _currentDungeon->render(window);

    // Player
    _player->render(window);
}
