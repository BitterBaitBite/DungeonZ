#include "Map/EnemyRoom.h"

#include <Assets/SpriteSheet.h>

#include "Core/AssetManager.h"
#include "Enums/EnemyType.h"
#include "Gameplay/TorchGoblin.h"
#include "Gameplay/Villager.h"
#include "Utils/Constants.h"

EnemyRoom::EnemyRoom(uint8_t enemyCount = 1, EnemyType type = EnemyType::Default) : _enemyType(type) {
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

    for (Enemy* enemy : _enemyPool) {
        if (enemy == nullptr) continue;
        delete enemy;
        enemy = nullptr;
    }
    _enemyPool.clear();

    for (Enemy* enemy : _activeEnemies) {
        if (enemy == nullptr) continue;
        enemy = nullptr;
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
                if (enemy->IsDead()) {
                    delete enemy;
                    return true;
                }
                return false;
            }
        ), _activeEnemies.end()
    );
}

void EnemyRoom::update(float deltaMilliseconds) {
    Room::update(deltaMilliseconds);

    clearDeadEnemies();

    for (Enemy* enemy : _activeEnemies) {
        if (enemy == nullptr) continue;

        enemy->update(deltaMilliseconds);
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
    Enemy::EnemyInfo* enemyInfo;

    // TODO: put the respective getters correctly
    switch (_enemyType) {
        case EnemyType::Villager:
            enemyInfo = &getVillagerInfo();
            break;

        case EnemyType::TorchGoblin:
            enemyInfo = &getTorchGoblinInfo();
            break;

        case EnemyType::Default:
        default:
            enemyInfo = &getDefaultEnemyInfo();
            break;
    }

    for (Enemy* enemy : _enemyPool) {
        if (enemy == nullptr) continue;

        // TODO: change position to random-generated in no-collision tiles
        enemyInfo->position = { BACKGROUND_ROW_SIZE * TILE_WIDTH / 2, BACKGROUND_COL_SIZE * TILE_HEIGHT / 2 };

        enemy->init(enemyInfo);
        _activeEnemies.push_back(enemy);
    }
}

Enemy::EnemyInfo EnemyRoom::getDefaultEnemyInfo() {
    SpriteSheet::SheetDescriptor enemySpriteInfo;
    enemySpriteInfo.path = "../Data/Images/Enemies/Pawn_Purple.png";
    enemySpriteInfo.rows = 6;
    enemySpriteInfo.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySpriteInfo);

    Enemy::EnemyInfo enemyInfo;
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    enemyInfo.texture = enemyTexture;
    enemyInfo.speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    enemyInfo.spriteHeight = (enemyInfo.texture->getSize().y / enemySpriteSheet->getRows()) * enemyInfo.scale.y;
    enemyInfo.spriteWidth = (enemyInfo.texture->getSize().x / enemySpriteSheet->getCols()) * enemyInfo.scale.x;
    enemyInfo.maxHealth = 3;

    return enemyInfo;
}

Villager::VillagerInfo EnemyRoom::getVillagerInfo() {
    SpriteSheet::SheetDescriptor enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Pawn_Red.png";
    enemySheetDesc.rows = 6;
    enemySheetDesc.cols = 6;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    Villager::VillagerInfo villagerInfo;
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    villagerInfo.texture = enemyTexture;
    villagerInfo.speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    villagerInfo.spriteHeight = villagerInfo.texture->getSize().y / enemySpriteSheet->getRows() * villagerInfo.scale.y;
    villagerInfo.spriteWidth = villagerInfo.texture->getSize().x / enemySpriteSheet->getCols() * villagerInfo.scale.x;
    villagerInfo.maxHealth = 3;
    villagerInfo.attackDamage = 2;

    return villagerInfo;
}

TorchGoblin::TorchGoblinInfo EnemyRoom::getTorchGoblinInfo() {
    SpriteSheet::SheetDescriptor enemySheetDesc;
    enemySheetDesc.path = "../Data/Images/Enemies/Torch_Red.png";
    enemySheetDesc.rows = 5;
    enemySheetDesc.cols = 7;
    auto enemySpriteSheet = new SpriteSheet(enemySheetDesc);

    TorchGoblin::TorchGoblinInfo torchGoblinInfo;
    sf::Texture* enemyTexture = AssetManager::getInstance()->loadTexture(enemySpriteSheet->getPath());
    torchGoblinInfo.texture = enemyTexture;
    torchGoblinInfo.speed = { 100.f * MILLISECONDS_TO_SECONDS, 100.f * MILLISECONDS_TO_SECONDS };
    torchGoblinInfo.spriteHeight = enemyTexture->getSize().y / enemySpriteSheet->getRows() * torchGoblinInfo.scale.y;

    torchGoblinInfo.spriteWidth = enemyTexture->getSize().x / enemySpriteSheet->getCols() * torchGoblinInfo.scale.x;

    torchGoblinInfo.maxHealth = 5;
    torchGoblinInfo.attackDamage = 1;

    return torchGoblinInfo;
}
