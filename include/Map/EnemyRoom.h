#pragma once
#include "Room.h"
#include "Enums/EnemyType.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/TorchGoblin.h"
#include "Gameplay/Villager.h"

enum class EnemyType;

class EnemyRoom : public Room {
    public:
        EnemyRoom(uint8_t enemyCount = 1, EnemyType type = EnemyType::Default);
        ~EnemyRoom() override;

        void init(std::vector<DirectionEnum> adjacentRooms) override;
        void clearDeadEnemies();
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    protected:
        EnemyType _enemyType { EnemyType::Default };
        std::vector<Enemy*> _enemyPool;
        std::vector<Enemy*> _activeEnemies;
        void initializeEnemies();
        Enemy::EnemyInfo* getDefaultEnemyInfo();
        Villager::VillagerInfo* getVillagerInfo();
        TorchGoblin::TorchGoblinInfo* getTorchGoblinInfo();

    public:
        uint8_t getTotalEnemies() const { return _enemyPool.size(); }
        std::vector<Enemy*> getActiveEnemies() const { return _activeEnemies; }
};
