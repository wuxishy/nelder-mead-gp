#ifndef __ROSENBROCK_HPP
#define __ROSENBROCK_HPP

#include "nelder_mead.hpp"
#include "rng.hpp"

struct rosenbrock : public cost_function {
    coord displace;

    rosenbrock (coord& dis) : cost_function(2), displace(dis) {}
    virtual ~rosenbrock() override = default;

    virtual double operator() (coord& x) override;
};

simplex initialize_rosenbrock();

#endif // __ROSENBROCK_HPP
