#include "Utils/Random.h"

#include <random>

int Random::randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

int Random::randomIntExc(int min, int max) {
    return randomInt(min, max - 1);
}

float Random::randomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    return dis(gen);
}
