#include <Book/Randomizer.hpp>

Randomizer::Randomizer(int min, int max) : gen(rd()), dis(min, max) {}

int Randomizer::get() {
    return dis(gen);
}