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
    vw,     // worst vertex
    vsb,    // second best vertex
    vsw,    // second worse vertex
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

    std::vector<Node*> children;
};

struct cost_function {
    int dim;

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

    std::map<double, coord> pts;
    coord centroid;

    simplex (int d, cost_function* f, std::vector<coord> initial);

    coord& get_pt(terminal x);
    coord eval(Node* root);
    void replace_worst(coord np);
};

#endif // __NELDER_MEAD_HPP
