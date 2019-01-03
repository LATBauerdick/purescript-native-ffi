
#include <random>
#include <cmath>
#include "purescript.h"

FOREIGN_BEGIN( Effect_Random )
//-- | Returns a random number between 0 (inclusive) and 1 (exclusive).
// foreign import random :: Effect Number
exports["random"] = []() -> boxed {
    static std::uniform_real_distribution<double> distribution(0, 1);
    static std::random_device dev;
    static std::mt19937_64 generator(static_cast<uint64_t>(dev()) << 32 | dev());
    return distribution(generator);
};

FOREIGN_END
