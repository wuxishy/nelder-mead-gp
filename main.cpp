#include <omp.h>

#include "gp.hpp"
#include "quadratic.hpp"

int main() {
    #pragma omp parallel for
    for(int i = 0; i < 10; ++i) {
        simplex s = initialize_quadratic(10);
        // do stuff
        delete s.cf;
    }
    
    return 0;
}
