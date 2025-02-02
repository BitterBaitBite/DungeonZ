#include "Map/Dungeon.h"

#include <iostream>
#include <random>

#include "Core/DungeonManager.h"
#include "Enums/Difficulty.h"
#include "Enums/DirectionEnum.h"
#include "Enums/EnemyType.h"
#include "Enums/RoomType.h"
#include "Map/EnemyRoom.h"
#include "Utils/Random.h"

Dungeon::Dungeon() {
    init();

    GenerateRooms();
    DebugDungeon();
    InitializeRooms();
}

void Dungeon::clearDungeon() {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            delete _rooms[i][j];
            _rooms[i][j] = nullptr;
        }
    }
}

Dungeon::~Dungeon() {
    clearDungeon();
}

void Dungeon::render(sf::RenderWindow& window) {
    _rooms[_currentRoom.x][_currentRoom.y]->render(window);
}

void Dungeon::update(float deltaMilliseconds) {
    _rooms[_currentRoom.x][_currentRoom.y]->update(deltaMilliseconds);
}

void Dungeon::reset() {
    clearDungeon();
    init();

    GenerateRooms();
    DebugDungeon();
    InitializeRooms();
}

void Dungeon::init() {
    for (int i = 0; i < DUNGEON_ROOMS; ++i) {
        for (int j = 0; j < DUNGEON_ROOMS; ++j) {
            _rooms[i][j] = nullptr;
        }
    }
}

Room* Dungeon::getCurrentRoom() const {
    if (_currentRoom.x < 0
        || _currentRoom.y < 0
        || _currentRoom.x >= DUNGEON_ROOMS
        || _currentRoom.y >= DUNGEON_ROOMS
    ) {
        return nullptr;
    }

    return _rooms[_currentRoom.x][_currentRoom.y];
}

Room* createRoom(RoomType type) {
    switch (type) {
        case RoomType::EnemyRoom: {
            // Random type enemies spawned: 2 -> 4 -> 6
            Difficulty difficulty = DungeonManager::getInstance()->getDifficulty();
            uint8_t enemyCount = BASE_ENEMY_SPAWN + DIFFICULTY_SPAWN_MULTIPLIER * static_cast<uint8_t>(difficulty);
            int enemyTypeIndex = Random::randomIntExc(0, static_cast<int>(EnemyType::Count));

            return new EnemyRoom(enemyCount, static_cast<EnemyType>(enemyTypeIndex));
        }

        case RoomType::Default:
        default:
            return new Room();
    }
}

void Dungeon::GenerateRooms() {
    for (int roomCount = 0; roomCount < DUNGEON_ROOMS;) {
        int row = Random::randomInt(0, DUNGEON_ROOMS - 1);
        int col = Random::randomInt(0, DUNGEON_ROOMS - 1);

        // Gets a random room type
        auto roomType = static_cast<RoomType>(Random::randomInt(0, static_cast<int>(RoomType::Count)));

        if (roomCount == 0) {
            _currentRoom = { row, col };
            _rooms[row][col] = new Room();
            roomCount++;
        }
        else if (_rooms[row][col] == nullptr && HasAdjacentRoom(row, col)) {
            _rooms[row][col] = createRoom(roomType);
            roomCount++;
        }
    }

    int roomCount = 0;
    for (int i = 0; i < DUNGEON_ROOMS; ++i) {
        for (int j = 0; j < DUNGEON_ROOMS; ++j) {
            if (_rooms[i][j] != nullptr) { roomCount++; }
        }
    }
}

void Dungeon::InitializeRooms() {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                _rooms[i][j]->init(GetAdjacentRooms(i, j));
            }
        }
    }
}

void Dungeon::DebugDungeon() {
    std::string dungeonString = "  ----------------------------------\n";

    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        std::string rowString = std::to_string(i) + " | ";

        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                rowString += _rooms[i][j]->toString(i == _currentRoom.x && j == _currentRoom.y);
            }
            else rowString += "   ";
        }

        rowString += " |\n";
        dungeonString += rowString;
    }
    dungeonString += "  ----------------------------------\n";

    std::cout << dungeonString;
}

bool Dungeon::HasRoom(int row, int col) const {
    return (
        row >= 0
        && row < DUNGEON_ROOMS
        && col >= 0
        && col < DUNGEON_ROOMS
        && _rooms[row][col] != nullptr
    );
}

bool Dungeon::HasAdjacentRoom(int row, int col) {
    // Has top room
    if (row - 1 >= 0 && _rooms[row - 1][col] != nullptr) return true;

    // Has bottom room
    if (row + 1 < DUNGEON_ROOMS && _rooms[row + 1][col] != nullptr) return true;

    // Has left room
    if ((col - 1 >= 0) && _rooms[row][col - 1] != nullptr) return true;

    // Has right room
    if (col + 1 < DUNGEON_ROOMS && _rooms[row][col + 1] != nullptr) return true;

    return false;
}

bool Dungeon::HasAdjacentRoom(DirectionEnum direction) const {
    switch (direction) {
        case DirectionEnum::Right: {
            if (_currentRoom.y + 1 < DUNGEON_ROOMS && _rooms[_currentRoom.x][_currentRoom.y + 1] != nullptr)
                return true;
            return false;
        }

        case DirectionEnum::Left: {
            if (_currentRoom.y - 1 >= 0 && _rooms[_currentRoom.x][_currentRoom.y - 1] != nullptr) return true;
            return false;
        }

        case DirectionEnum::Up: {
            if ((_currentRoom.x - 1 >= 0) && _rooms[_currentRoom.x - 1][_currentRoom.y] != nullptr) return true;
            return false;
        }

        case DirectionEnum::Down: {
            if (_currentRoom.x + 1 < DUNGEON_ROOMS && _rooms[_currentRoom.x + 1][_currentRoom.y] != nullptr)
                return true;
            return false;
        }

        default:
            return false;
    }
}

bool Dungeon::HasAdjacentRoom(int row, int col, DirectionEnum direction) const {
    switch (direction) {
        case DirectionEnum::Right: {
            if (col + 1 < DUNGEON_ROOMS && _rooms[row][col + 1] != nullptr)
                return true;
            return false;
        }

        case DirectionEnum::Left: {
            if (col - 1 >= 0 && _rooms[row][col - 1] != nullptr) return true;
            return false;
        }

        case DirectionEnum::Up: {
            if (row - 1 >= 0 && _rooms[row - 1][col] != nullptr) return true;
            return false;
        }

        case DirectionEnum::Down: {
            if (row + 1 < DUNGEON_ROOMS && _rooms[row + 1][col] != nullptr)
                return true;
            return false;
        }

        default:
            return false;
    }
}

std::vector<DirectionEnum> Dungeon::GetAdjacentRooms(int row, int col) const {
    std::vector<DirectionEnum> adjacentRooms;
    for (DirectionEnum direction : AllDirections) {
        if (HasAdjacentRoom(row, col, direction)) {
            adjacentRooms.push_back(direction);
        }
    }

    return adjacentRooms;
}

bool Dungeon::moveTo(DirectionEnum direction) {
    switch (direction) {
        case DirectionEnum::Right: {
            _currentRoom.y += 1;
            DebugDungeon();
            return true;
        }

        case DirectionEnum::Left: {
            _currentRoom.y -= 1;
            DebugDungeon();
            return true;
        }

        case DirectionEnum::Up: {
            _currentRoom.x -= 1;
            DebugDungeon();
            return true;
        }

        case DirectionEnum::Down: {
            _currentRoom.x += 1;
            DebugDungeon();
            return true;
        }
    }

    DebugDungeon();
    return false;
}

// Gets a vector with x = active enemies / y = total enemies
sf::Vector2i Dungeon::getEnemyCount() const {
    sf::Vector2i enemyCount(0, 0);
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            auto enemyRoom = dynamic_cast<EnemyRoom*>(_rooms[i][j]);
            if (enemyRoom == nullptr) continue;

            enemyCount.x += enemyRoom->getCurrentEnemies();
            enemyCount.y += enemyRoom->getTotalEnemies();
        }
    }

    return enemyCount;
}
