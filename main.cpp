#include <omp.h>

#include "gp.hpp"
#include "quadratic.hpp"

int main() {
    population p = population(200, 6);

    for(int i = 0; i < 1; ++i) {
        simplex s = initialize_quadratic(5);
        p.training_set.push_back(s);
    }

    p.initialize();

    p.run(500);

    for(simplex s : p.training_set) delete s.cf; 
    
    return 0;
}
