/********************************
 * Implements basic features of *
 * Nelder-Mead Algorithm        *
 * Author: Haoyuan Sun          *
 ********************************/


#ifndef __NELDER_MEAD_HPP
#define __NELDER_MEAD_HPP

#include <vector>

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
    std::pair<int, int> t;
    std::vector<Node*> children;

    Node(int a, int b) : t({a, b}) {};
};

namespace primitive {
    coord ifelse(double (*f)(coord&), coord& a, coord& b, coord& c, coord& d);
    coord refl(coord& a, coord& b);
    coord expan(coord& a, coord& b);
    coord contr(coord& a, coord& b);
}

#endif // __NELDER_MEAD_HPP
