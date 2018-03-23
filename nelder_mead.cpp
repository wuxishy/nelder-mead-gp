/*****************************
 * Implements basic features *
 * of Nelder-Mead Algorithm  *
 * Author: Haoyuan Sun       *
 *****************************/

#include "nelder_mead.hpp"

coord primitive::ifelse(double (*f)(coord&), coord& a, coord& b,
        coord& c, coord& d) {
    if (f(a) < f(b)) return c;
    else return d;
}

coord primitive::refl(coord& a, coord& b) {
    coord ret;

    for(size_t i = 0; i < a.size(); ++i)
        ret.push_back(2 * a[i] - b[i]);

    return ret;
}

coord primitive::expan(coord& a, coord& b) {
    coord ret;

    for(size_t i = 0; i < a.size(); ++i)
        ret.push_back(3 * a[i] - b[i]);

    return ret;
}

coord primitive::contr(coord& a, coord& b) {
    coord ret;

    for(size_t i = 0; i < a.size(); ++i)
        ret.push_back(0.5 * a[i] + b[i]);

    return ret;
}
