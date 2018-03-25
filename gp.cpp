#include "gp.hpp"

population::population(int s, int dep) : size(s) {
    roots = std::vector<Node*>(s);

    for (int i = 0; i < size; ++i) {
        roots[i] = new Node();
        
        if (i%2) grow(nullptr, roots[i], (i/2) % dep + 1);
        else full(nullptr, roots[i], (i/2) % dep + 1);
    }
}

population::~population() {
   for (Node* n : roots) clear(n); 
}

void population::clear(Node* cur) {
    for (Node* n : cur->children) clear(n);

    delete cur;
}

void population::grow(Node* parent, Node* cur, int dep) {
    if (dep == 1 || r3() < 0.2) {
        cur->t = type::terminal;
        cur->id = r2();
        return;
    }
    
    cur->t = type::func;
    cur->id = r1();

    int nc = (cur->id == 1) ? 4 : 2;
    for(int i = 0; i < nc; ++i) {
        Node* tmp = new Node();
        cur->children.push_back(tmp);
        grow(cur, tmp, dep-1);
    }
}

void population::full(Node* parent, Node* cur, int dep) {
    if (dep == 1) {
        cur->t = type::terminal;
        cur->id = r2();
        return;
    }
    
    cur->t = type::func;
    cur->id = r1();

    int nc = (cur->id == 1) ? 4 : 2;
    for(int i = 0; i < nc; ++i) {
        Node* tmp = new Node();
        cur->children.push_back(tmp);
        full(cur, tmp, dep-1);
    }
}
