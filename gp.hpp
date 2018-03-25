#ifndef __GP_HPP
#define __GP_HPP

#include "nelder_mead.hpp"
#include "rng.hpp"

struct population {
    int size;
    std::vector<Node*> roots;
    
    randint r1 = randint(1, 4), r2 = randint(1, 5);
    randreal r3;

    population(int s, int dep);
    ~population();

    void clear(Node* cur);

    void grow(Node* parent, Node* cur, int dep);

    void full(Node* parent, Node* cur, int dep);
};

#endif // __GP_HPP
