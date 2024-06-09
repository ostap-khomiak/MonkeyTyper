#include "word.h"

auto Word::getRandomWord(const std::vector<std::string>& words) -> std::string {
    srand(time(NULL));
    return words[rand() % words.size()];
}