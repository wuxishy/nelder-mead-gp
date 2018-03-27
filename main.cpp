#include <omp.h>

#include "gp.hpp"
#include "quadratic.hpp"

int main() {
    population p = population(200, 5);

    for(int i = 0; i < 10; ++i) {
        simplex s = initialize_quadratic(10);
        p.training_set.push_back(s);
    }

    p.run(100);

    for(simplex s : p.training_set) delete s.cf; 
    
    return 0;
}
