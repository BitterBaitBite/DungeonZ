#include "Map/EnemyRoom.h"

#include <Assets/SpriteSheet.h>

#include "Core/AssetManager.h"
#include "Core/WindowManager.h"
#include "Enums/EnemyType.h"
#include "Gameplay/TorchGoblin.h"
#include "Gameplay/Villager.h"
#include "Utils/Constants.h"

EnemyRoom::EnemyRoom(uint8_t enemyCount, EnemyType type) : _enemyType(type) {
    Enemy* enemy = nullptr;
    for (int i = 0; i < enemyCount; i++) {
        switch (type) {
            case EnemyType::Villager:
                enemy = new Villager();
                break;

            case EnemyType::TorchGoblin:
                enemy = new TorchGoblin();
                break;

            case EnemyType::Default:
            default:
                enemy = new Enemy();
                break;
        }
        _enemyPool.push_back(enemy);
        enemy = nullptr;
    }
}

EnemyRoom::~EnemyRoom() {
    Room::~Room();

    for (size_t i = 0; i < _enemyPool.size(); ++i) {
        if (_enemyPool[i] == nullptr) continue;
        delete _enemyPool[i];
        _enemyPool[i] = nullptr;
    }
    _enemyPool.clear();

    for (size_t i = 0; i < _activeEnemies.size(); ++i) {
        if (_activeEnemies[i] == nullptr) continue;
        _activeEnemies[i] = nullptr;
    }
    _activeEnemies.clear();
}

void EnemyRoom::init(std::vector<DirectionEnum> adjacentRooms) {
    Room::init(adjacentRooms);

    initializeEnemies();
}

void EnemyRoom::clearDeadEnemies() {
    _activeEnemies.erase(
        std::remove_if(
            _activeEnemies.begin(),
            _activeEnemies.end(),
            [](Enemy* enemy) {
                return enemy->IsDead();
            }
        ), _activeEnemies.end()
    );
}

void EnemyRoom::update(uint32_t deltaTime) {
    Room::update(deltaTime);

    clearDeadEnemies();

    for (Enemy* enemy : _activeEnemies) {
        if (enemy == nullptr) continue;

        enemy->update(deltaTime);
    }
}

void EnemyRoom::render(sf::RenderWindow& window) {
    Room::render(window);

    for (Enemy* enemy : _activeEnemies) {
        if (enemy == nullptr) continue;

        enemy->render(window);
    }
}

void EnemyRoom::initializeEnemies() {
    Enemy::EnemyInfo* enemyInfo { nullptr };

    // TODO: put the respective getters correctly
    switch (_enemyType) {
        case EnemyType::Villager:
            enemyInfo = getVillagerInfo();
            break;

        case EnemyType::TorchGoblin:
            enemyInfo = getTorchGoblinInfo();
            break;

        case EnemyType::Default:
        default:
            enemyInfo = getDefaultEnemyInfo();
            break;
    }

    for (Enemy* enemy : _enemyPool) {
        if (enemy == nullptr) continue;

        // TODO: change position to random-generated in no-collision tiles
        enemyInfo->position = { BACKGROUND_ROW_SIZE * TILE_WIDTH / 2, BACKGROUND_COL_SIZE * TILE_HEIGHT / 2 };

        enemy->init(enemyInfo);
        _activeEnemies.push_back(enemy);
    }

    delete enemyInfo;
}

Enemy::EnemyInfo* EnemyRoom::getDefaultEnemyInfo() {
    SpriteSheet::SpriteSheetInfo enemySpriteInfo;
    enemySpriteInfo.path = "../Data/Images/Enemies/Pawn_Purple.png";
    enemySpriteInfo.rows = 6;
    enemySpriteInfo.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySpriteInfo);

    auto enemyInfo = new Enemy::EnemyInfo();
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    enemyInfo->texture = enemyTexture;
    enemyInfo->speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    enemyInfo->spriteHeight = (enemyInfo->texture->getSize().y / enemySpriteSheet->getRows()) * enemyInfo->scale.y;
    enemyInfo->spriteWidth = (enemyInfo->texture->getSize().x / enemySpriteSheet->getCols()) * enemyInfo->scale.x;
    enemyInfo->maxHealth = 3;

    return enemyInfo;
}

Villager::VillagerInfo* EnemyRoom::getVillagerInfo() {
    SpriteSheet::SpriteSheetInfo enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Pawn_Red.png";
    enemySheetDesc.rows = 6;
    enemySheetDesc.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    auto villagerInfo = new Villager::VillagerInfo();
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    villagerInfo->texture = enemyTexture;
    villagerInfo->speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    villagerInfo->spriteHeight = villagerInfo->texture->getSize().y / enemySpriteSheet->getRows() * villagerInfo->scale.
        y;
    villagerInfo->spriteWidth = villagerInfo->texture->getSize().x / enemySpriteSheet->getCols() * villagerInfo->scale.
        x;
    villagerInfo->maxHealth = 3;
    villagerInfo->attackDamage = 2;

    return villagerInfo;
}

TorchGoblin::TorchGoblinInfo* EnemyRoom::getTorchGoblinInfo() {
    SpriteSheet::SpriteSheetInfo enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Torch_Red.png";
    enemySheetDesc.rows = 5;
    enemySheetDesc.cols = 7;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    auto torchGoblinInfo = new TorchGoblin::TorchGoblinInfo();
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    torchGoblinInfo->texture = enemyTexture;
    torchGoblinInfo->speed = { 80.f * MILLISECONDS_TO_SECONDS, 80.f * MILLISECONDS_TO_SECONDS };
    torchGoblinInfo->position = {
        WindowManager::getInstance()->getWindowCenter().x - TILE_WIDTH * 2,
        WindowManager::getInstance()->getWindowCenter().y
    };
    torchGoblinInfo->spriteHeight = torchGoblinInfo->texture->getSize().y / enemySpriteSheet->getRows() *
        torchGoblinInfo->
        scale.y;
    torchGoblinInfo->spriteWidth = torchGoblinInfo->texture->getSize().x / enemySpriteSheet->getCols() * torchGoblinInfo
        ->
        scale.x;
    torchGoblinInfo->maxHealth = 5;
    torchGoblinInfo->attackDamage = 1;
    torchGoblinInfo->fireDamage = 1;
    torchGoblinInfo->fireDuration = 3.f;

    return torchGoblinInfo;
}
