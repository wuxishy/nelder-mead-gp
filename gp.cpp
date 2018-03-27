#include "gp.hpp"

#include "omp.h"

population::population(int s, int dep) : size(s) {
    roots = std::vector<std::pair<double, Node*>>(s);

    #pragma omp parallel for 
    for (int i = 0; i < size; ++i) {
        roots[i].second = new Node();
        
        if (i%2) grow(nullptr, roots[i].second, (i/2) % dep + 1);
        else full(nullptr, roots[i].second, (i/2) % dep + 1);

        roots[i].first = score(roots[i].second);
    }

    std::sort(roots.begin(), roots.end());
}

population::~population() {
   for (auto n : roots) clear(n.second); 
}

void population::clear(Node* cur) {
    for (Node* n : cur->children) clear(n);

    delete cur;
}

double population::score(Node* root) {
    double ret = 0;
    for (simplex s : training_set) 
        ret += s.make_copy().compute(root) / training_set.size();

    if (root->num_func + root->num_term > 20) ret *= 100;

    return ret;
}

void population::grow(Node* parent, Node* cur, int dep) {
    if (dep == 1 || r3() < 0.2) {
        cur->parent = parent;
        cur->t = type::terminal;
        cur->id = r2();
        cur->num_term = 1;
        cur->num_func = 0;
        return;
    }
    
    cur->parent = parent;
    cur->t = type::func;
    cur->id = r1();
    cur->num_term = 0;
    cur->num_func = 1;

    int nc = (cur->id == 1) ? 4 : 2;
    for(int i = 0; i < nc; ++i) {
        Node* tmp = new Node();
        cur->children.push_back(tmp);
        grow(cur, tmp, dep-1);
        cur->num_term += tmp->num_term;
        cur->num_func += tmp->num_func;
    }
}

void population::full(Node* parent, Node* cur, int dep) {
    if (dep == 1) {
        cur->parent = parent;
        cur->t = type::terminal;
        cur->id = r2();
        cur->num_term = 1;
        cur->num_func = 0;
        return;
    }
    
    cur->parent = parent;
    cur->t = type::func;
    cur->id = r1();
    cur->num_term = 0;
    cur->num_func = 1;

    int nc = (cur->id == 1) ? 4 : 2;
    for(int i = 0; i < nc; ++i) {
        Node* tmp = new Node();
        cur->children.push_back(tmp);
        full(cur, tmp, dep-1);
        cur->num_term += tmp->num_term;
        cur->num_func += tmp->num_func;
    }
}

Node* population::make_copy(Node* root) {
    Node* new_node = new Node();

    new_node->parent = root->parent;
    new_node->t = root->t;
    new_node->id = root->id;
    new_node->num_term = root->num_term;
    new_node->num_func = root->num_func;

    for(size_t i = 0; i < root->children.size(); ++i)
        new_node->children.push_back(make_copy(root->children[i]));

    return new_node;
}

Node* population::select(Node* cur, int num, bool t) {
    if (num == 0) return cur;

    if (t) --num;
    for(size_t i = 0; i < cur->children.size(); ++i) {
        int n = t ? cur->children[i]->num_func : cur->children[i]->num_term;
        if (num < n) return select(cur->children[i], num, t);
        else num -= n;
    }
    
    return nullptr;
}

void population::propagate(Node* cur) {
    if (cur == nullptr) return;

    cur->num_func = 1;
    cur->num_term = 0;

    for(Node* n : cur->children) {
        cur->num_func += n->num_func;
        cur->num_term += n->num_term;
    }
}

void population::crossover(Node* a, Node* b) {
    // do nothing with trivial trees
    if (a->num_func == 0 || b->num_func == 0) return;

    Node *ca, *cb;
    
    if(r3() < 0.1) {
        randint rn = randint(0, a->num_term-1);
        ca = select(a, rn(), true);
    }
    else {
        randint rn = randint(1, a->num_func-1);
        ca = select(a, rn());
    }
    
    if(r3() < 0.1) {
        randint rn = randint(0, b->num_term-1);
        cb = select(b, rn(), true);
    }
    else {
        randint rn = randint(1, b->num_func-1);
        cb = select(b, rn());
    }

    for(size_t i = 0; i < ca->parent->children.size(); ++i)
        if (ca->parent->children[i] == ca) ca->parent->children[i] = cb;
    for(size_t i = 0; i < cb->parent->children.size(); ++i)
        if (cb->parent->children[i] == cb) cb->parent->children[i] = ca;

    Node *tmp = ca->parent;
    ca->parent = cb->parent;
    cb->parent = tmp;

    propagate(ca->parent);
    propagate(cb->parent);
}

void population::breed() {
    int cut = size / 10;
    
    std::vector<Node*> buf;
    randint randp = randint(0, size-cut-1);
    for(int i = 0; i < cut*2; ++i) {
        int n = randp(), m = randp();

        if (roots[n].first < roots[m].first)
            buf.push_back(roots[n].second);
        else
            buf.push_back(roots[m].second);
    }
    for(int i = 0; i < cut; ++i) crossover(buf[2*i], buf[2*i+1]);
    
    for(int i = 0; i < cut; ++i) {
        clear(roots.back().second);
        roots.pop_back();
    }

    #pragma omp parallel for
    for(int i = 0; i < cut*2; ++i) {
        roots.push_back({score(buf[i]), buf[i]});
    }

    std::sort(roots.begin(), roots.end());

    for(int i = 0; i < cut; ++i) {
        clear(roots.back().second);
        roots.pop_back();
    }
}

void population::run(int gen) {
    // no fancy stuff like editing and decimation
    for(int i = 0; i < gen; ++i) {
        breed();
    }
}
