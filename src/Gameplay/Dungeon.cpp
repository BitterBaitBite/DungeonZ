#include "Gameplay/Dungeon.h"

#include <iostream>
#include <random>

#include "Enums/DirectionEnum.h"
#include "Utils/Random.h"

Dungeon::Dungeon() {
    init();

    GenerateRooms();
    DebugDungeon();
    InitializeRooms();
}

Dungeon::~Dungeon() {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            delete _rooms[i][j];
            _rooms[i][j] = nullptr;
        }
    }
}

void Dungeon::render(sf::RenderWindow& window) {
    _rooms[_currentRoom.x][_currentRoom.y]->render(window);
}

void Dungeon::update(float deltaMilliseconds) {
    _rooms[_currentRoom.x][_currentRoom.y]->update(deltaMilliseconds);
}

void Dungeon::init() {
    for (int i = 0; i < DUNGEON_ROOMS; ++i) {
        for (int j = 0; j < DUNGEON_ROOMS; ++j) {
            _rooms[i][j] = nullptr;
        }
    }
}

void Dungeon::GenerateRooms() {
    for (int roomCount = 0; roomCount < DUNGEON_ROOMS;) {
        int row = Random::randomInt(0, DUNGEON_ROOMS - 1);
        int col = Random::randomInt(0, DUNGEON_ROOMS - 1);

        if (roomCount == 0) {
            _currentRoom = { row, col };
            _rooms[row][col] = new Room();
            roomCount++;
        }
        else if (_rooms[row][col] == nullptr && HasAdjacentRoom(row, col)) {
            _rooms[row][col] = new Room();
            roomCount++;
        }
    }

    int roomCount = 0;
    for (int i = 0; i < DUNGEON_ROOMS; ++i) {
        for (int j = 0; j < DUNGEON_ROOMS; ++j) {
            if (_rooms[i][j] != nullptr) { roomCount++; }
        }
    }

    printf("%i\n", roomCount);
}

void Dungeon::InitializeRooms() {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                _rooms[i][j]->init();
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

    std::cout << '\n' << dungeonString << '\n';
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

bool Dungeon::HasRoom(int row, int col) const {
    return (
        row >= 0
        && row < DUNGEON_ROOMS
        && col >= 0
        && col < DUNGEON_ROOMS
        && _rooms[row][col] != nullptr
    );
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
