#include "quadratic.hpp"

double quadratic::operator() (coord& x) {
    double ret = 0;

    for(int i = 0; i < dim; ++i)
        ret += (x[i] - displace[i]) * (x[i] - displace[i]);

    return ret;
} 

simplex initialize_quadratic(int dim) {
    randint r = randint(-100, 100);

    coord dis;
    for(int i = 0; i < dim; ++i) dis.push_back(r());

    cost_function* cf = new quadratic(dim, dis);

    std::vector<coord> initial(dim+1, dis);
    for(int i = 0; i <= dim; ++i)
        for(int j = 0; j < dim; ++j)
            initial[i][j] += r();

    return simplex(dim, cf, initial);
}
