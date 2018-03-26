#ifndef __QUADRATIC_HPP
#define __QUADRATIC_HPP

#include "nelder_mead.hpp"
#include "rng.hpp"

struct quadratic : public cost_function {
    coord displace;

    quadratic (int d, coord& dis) : cost_function(d), displace(dis) {}
    virtual ~quadratic() override = default;

    virtual double operator() (coord& x) override;
};

simplex initialize_quadratic(int dim);

#endif // __QUADRATIC_HPP
