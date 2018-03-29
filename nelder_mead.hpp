/********************************
 * Implements basic features of *
 * Nelder-Mead Algorithm        *
 * Author: Haoyuan Sun          *
 ********************************/


#ifndef __NELDER_MEAD_HPP
#define __NELDER_MEAD_HPP

#include <vector>
#include <map>

using coord = std::vector<double>;

enum class type {
    func = 1,
    terminal = 2
};

enum class terminal {
    vb = 1, // best vertex
    vsb,    // second best vertex
    vsw,    // second worse vertex
    vw,     // worst vertex
    cent    // centroid 
};

enum class func {
    ifelse = 1,
    refl,
    expan,
    contr
};

struct Node {
    type t;
    int id;

    int num_term, num_func;

    Node* parent;
    std::vector<Node*> children;

    ~Node();

    void edit();
    void propagate();
};


struct cost_function {
    int dim;
    
    cost_function(int d) : dim(d) {}
	virtual ~cost_function() {}

    virtual double operator() (coord& x) = 0; 
};

namespace primitive {
    coord ifelse(cost_function* f, coord& a, coord& b, 
            coord& c, coord& d);
    coord refl(coord& a, coord& b);
    coord expan(coord& a, coord& b);
    coord contr(coord& a, coord& b);
}

struct simplex {
    int dim;
    cost_function* cf;

    std::multimap<double, coord> pts;
    coord centroid;

    simplex () {};
    simplex (int d, cost_function* f, std::vector<coord> initial);

    simplex make_copy();

    coord& get_pt(terminal x);
    coord eval(Node* root);
    void replace_worst(coord np);

    double compute(Node* root);
};

#endif // __NELDER_MEAD_HPP
