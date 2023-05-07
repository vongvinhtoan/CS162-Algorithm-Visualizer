#include <Book/Randomizer.hpp>

Randomizer::Randomizer(int min, int max) : gen(rd()), dis(min, max) {}

int Randomizer::getVal() {
    return dis(gen);
}

int Randomizer::getVal(int min, int max) {
    auto temp = getVal();
    return min + (temp % (max - min + 1));
}