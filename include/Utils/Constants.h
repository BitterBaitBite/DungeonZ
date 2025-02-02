#pragma once

// TODO: (OPTIMIZATION) change most int types to uint8_t
constexpr auto GAME_TITLE = "DungeonZ";
constexpr uint32_t FRAME_RATE_LIMIT = 60u;
constexpr float MILLISECONDS_TO_SECONDS = 1 / 1000.f;
constexpr float FRAMES_PER_SECOND = 10.f;

constexpr int ROOM_ROW_SIZE = 11;
constexpr int ROOM_COL_SIZE = 9;
constexpr int BACKGROUND_ROW_SIZE = ROOM_ROW_SIZE + 2;
constexpr int BACKGROUND_COL_SIZE = ROOM_COL_SIZE + 2;

constexpr int DUNGEON_ROOMS = 10;
constexpr int TILE_WIDTH = 64;
constexpr int TILE_HEIGHT = 64;

constexpr uint8_t BASE_ENEMY_SPAWN = 2;
constexpr uint8_t DIFFICULTY_SPAWN_MULTIPLIER = 2;

constexpr int ATTACK_COMBO_COUNT = 2;
