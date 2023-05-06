#include <Book/Randomizer.hpp>

Randomizer::Randomizer(int min, int max) : gen(rd()), dis(min, max) {}

int Randomizer::getVal() {
    return dis(gen);
}