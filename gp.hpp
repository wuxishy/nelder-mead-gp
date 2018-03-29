#ifndef __GP_HPP
#define __GP_HPP

#include <algorithm>

#include "nelder_mead.hpp"
#include "rng.hpp"

struct population {
    int size;
    int depth;
    std::vector<std::pair<double, Node*>> roots;

    std::vector<simplex> training_set;
    
    randint r1 = randint(1, 4), r2 = randint(1, 5);
    randreal r3;

    population(int s, int dep) : size(s), depth(dep) {} ;
    ~population();

    double score(Node* root);
    
    void initialize();
    void initialize(int len);

    void grow(Node* parent, Node* cur, int dep);

    void full(Node* parent, Node* cur, int dep);

    Node* make_copy(Node* root, Node* parent);

    Node* select(Node* cur, int num, bool t=false);

    void crossover(Node* a, Node* b);

    void breed();

    void run(int gen);
};

#endif // __GP_HPP
