#pragma once
#include <array>
#include <vector>

enum class DirectionEnum {
    Right,
    Down,
    Up,
    Left,
};

enum class FaceDirectionX {
    Right,
    Left,
};

enum class FaceDirectionY {
    Down,
    Up,
};

constexpr std::array<DirectionEnum, 4> AllDirections = {
    DirectionEnum::Right,
    DirectionEnum::Down,
    DirectionEnum::Up,
    DirectionEnum::Left
};
