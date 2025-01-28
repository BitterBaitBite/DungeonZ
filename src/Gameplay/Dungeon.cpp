#include "Gameplay/Dungeon.h"

#include <iostream>
#include <random>

#include "Enums/FaceDirection.h"

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
        }
    }
}

void Dungeon::setCurrentRoom(int row, int col) {
    _currentRoom.x = row;
    _currentRoom.y = col;
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
    // Random uniform distribution in the range of [0, DUNGEON_ROOMS]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, DUNGEON_ROOMS - 1);

    int roomCount = 0;
    for (int i = 0; i < DUNGEON_ROOMS;) {
        int row = dis(gen);
        int col = dis(gen);

        if (i == 0) {
            roomCount++;
            _currentRoom = { row, col };
            _rooms[row][col] = new Room();
            i++;
        }
        else if (_rooms[row][col] == nullptr && HasAdjacentRoom(row, col)) {
            roomCount++;
            _rooms[row][col] = new Room();
            i++;
        }
    }
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
    std::cout << "----------------------------------" << '\n';

    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        std::string rowString = "| ";

        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                rowString += _rooms[i][j]->toString(i == _currentRoom.x && j == _currentRoom.y);
            }
            else rowString += "   ";
        }

        std::cout << rowString << " |" << '\n';
    }

    std::cout << "----------------------------------" << '\n';
}

bool Dungeon::HasAdjacentRoom(int row, int col) {
    // Has left room
    if (row - 1 >= 0 && _rooms[row - 1][col] != nullptr) return true;

    // Has right room
    if (row + 1 < DUNGEON_ROOMS && _rooms[row + 1][col] != nullptr) return true;

    // Has top room
    if ((col - 1 >= 0) && _rooms[row][col - 1] != nullptr) return true;

    // Has bottom room
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

bool Dungeon::HasAdjacentRoom(FaceDirection direction) const {
    switch (direction) {
        case FaceDirection::Right: {
            if (_currentRoom.y + 1 < DUNGEON_ROOMS && _rooms[_currentRoom.x][_currentRoom.y + 1] != nullptr)
                return true;
            return false;
        }

        case FaceDirection::Left: {
            if (_currentRoom.y - 1 >= 0 && _rooms[_currentRoom.x][_currentRoom.y - 1] != nullptr) return true;
            return false;
        }

        case FaceDirection::Up: {
            if ((_currentRoom.x - 1 >= 0) && _rooms[_currentRoom.x - 1][_currentRoom.y] != nullptr) return true;
            return false;
        }

        case FaceDirection::Down: {
            if (_currentRoom.x + 1 < DUNGEON_ROOMS && _rooms[_currentRoom.x + 1][_currentRoom.y] != nullptr)
                return true;
            return false;
        }

        default:
            return false;
    }
}

bool Dungeon::moveTo(FaceDirection direction) {
    switch (direction) {
        case FaceDirection::Right: {
            _currentRoom.y += 1;
            DebugDungeon();
            return true;
        }

        case FaceDirection::Left: {
            _currentRoom.y -= 1;
            DebugDungeon();
            return true;
        }

        case FaceDirection::Up: {
            _currentRoom.x -= 1;
            DebugDungeon();
            return true;
        }

        case FaceDirection::Down: {
            _currentRoom.x += 1;
            DebugDungeon();
            return true;
        }
    }

    DebugDungeon();
    return false;
}
