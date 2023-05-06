#pragma once

#include <random>

class Randomizer {
    public:
        Randomizer(int min, int max);
        int getVal();

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<> dis;
};