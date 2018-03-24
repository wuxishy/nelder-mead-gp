#ifndef __RNG_HPP
#define __RNG_HPP

#include <random>

struct randint {
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

    randint (int a, int b) {
        std::random_device rd;
        gen.seed(rd());
        dis = std::uniform_int_distribution<> (a, b);
    }

    int operator() () {
        return dis(gen);
    }
};

struct rand {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    rand() {
        std::random_device rd;
        gen.seed(rd());
        dis = std::uniform_real_distribution<> (0, 1);
    }

    double operator() () {
        return dis(gen);
    }
};

#endif // __RNG_HPP
