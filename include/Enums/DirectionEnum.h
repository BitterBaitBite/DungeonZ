#pragma once
#include <array>
#include <vector>

enum class DirectionEnum {
    Right,
    Down,
    Up,
    Left,
};

enum class DirectionX {
    Right,
    Left,
};

enum class DirectionY {
    Down,
    Up,
};

constexpr std::array<DirectionEnum, 4> AllDirections = {
    DirectionEnum::Right,
    DirectionEnum::Down,
    DirectionEnum::Up,
    DirectionEnum::Left
};
