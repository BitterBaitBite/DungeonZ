#include "Gameplay/Dungeon.h"

#include <iostream>
#include <random>

Dungeon::Dungeon() {
    InitializeNull();
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

Room* Dungeon::getRoom(int row, int col) const {
    return _rooms[row][col];
}

void Dungeon::render(sf::RenderWindow& window) {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                _rooms[i][j]->render(window);
            }
        }
    }
}

void Dungeon::update(float deltaMilliseconds) {
    for (int i = 0; i < DUNGEON_ROOMS; i++) {
        for (int j = 0; j < DUNGEON_ROOMS; j++) {
            if (_rooms[i][j] != nullptr) {
                _rooms[i][j]->update(deltaMilliseconds);
            }
        }
    }
}

void Dungeon::InitializeNull() {
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

    /*
     *  Dungeon rooms disposition (row -> j, col -> k)
     *
     *             [j-1, k ]
     *                | |
     * [ j ,k-1] = [ j , k ] = [ j ,k+1]
     *                | |
     *             [j+1, k ]
     */

    int roomCount = 0;
    for (int i = 0; i < DUNGEON_ROOMS;) {
        int row = dis(gen);
        int col = dis(gen);

        if (i == 0) {
            roomCount++;
            // TODO player location = rooms[row, col] ?
            _rooms[row][col] = new Room();
            i++;

            // Debug
            // printf("[%i][%i]\n", row, col);
        }
        else if (_rooms[row][col] == nullptr && HasAdjacentRoom(row, col)) {
            roomCount++;
            _rooms[row][col] = new Room();
            i++;

            // Debug
            // printf("[%i][%i]\n", row, col);
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
                rowString += _rooms[i][j]->toString();
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
