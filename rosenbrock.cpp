#include "rosenbrock.hpp"

double rosenbrock::operator() (coord& x) {
    double ret = 0;

	ret += (x[0] - displace[0]) * (x[0] - displace[0]);
	ret += displace[1] * (x[1] - x[0]*x[0]) * (x[1] - x[0]*x[0]);

    return ret;
} 

simplex initialize_rosenbrock() {
	int dim = 2;
	
    randint r = randint(-50, 50);

    coord dis{0, 1};
    //for(int i = 0; i < dim; ++i) dis.push_back(r());

    cost_function* cf = new rosenbrock(dis);

    std::vector<coord> initial(dim+1, dis);
    for(int i = 0; i <= dim; ++i)
        for(int j = 0; j < dim; ++j)
            initial[i][j] += r();

    return simplex(dim, cf, initial);
}
